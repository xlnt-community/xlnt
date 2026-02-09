// Copyright (C) 2016-2022 Thomas Fussell
// Copyright (C) 2002-2007 Ariya Hidayat (ariya@kde.org).
// Copyright (c) 2024-2026 xlnt-community
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

#include <xlnt/utils/exceptions.hpp>
#include <detail/binary.hpp>
#include <detail/cryptography/compound_document.hpp>
#include <detail/unicode.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

// NOTE: compound files are not part of the OOXML specification (ECMA-376).
// This implementation is based on the "[MS-CFB]: Compound File Binary File Format" specification.
namespace {

using namespace xlnt::detail;

template <typename T>
std::string format_hex(T value)
{
    // Format example: 0x0000660F
    return fmt::format("0x{:08X}", value);
}

int compare_keys(const std::string &left, const std::string &right)
{
    auto to_lower = [](std::string s) {
        if (s.empty())
        {
            return s;
        }
        else
        {
            static const auto locale = std::locale();
            std::use_facet<std::ctype<char>>(locale).tolower(&s[0], &s[0] + s.size());

            return s;
        }
    };

    return to_lower(left).compare(to_lower(right));
}

std::vector<std::string> split_path(const std::string &path)
{
    auto split = std::vector<std::string>();
    auto current = path.find('/');
    auto prev = std::size_t(0);

    while (current != std::string::npos)
    {
        split.emplace_back(path.substr(prev, current - prev));
        prev = current + 1;
        current = path.find('/', prev);
    }

    split.emplace_back(path.substr(prev));

    return split;
}

std::string join_path(const std::vector<std::string> &path)
{
    auto joined = std::string();

    for (const auto &part : path)
    {
        joined.append(part);
        joined.push_back('/');
    }

    return joined;
}

template <typename T>
const T & back_safe(const std::vector<T> &vec)
{
    return vec.at(vec.size() - 1);
}

template <typename T>
T & back_safe(std::vector<T> &vec)
{
    return vec.at(vec.size() - 1);
}

const sector_id FreeSector = -1;
const sector_id EndOfChain = -2;
const sector_id SATSector = -3;
//const sector_id MSATSector = -4;

const directory_id End = -1;

} // namespace

namespace xlnt {
namespace detail {

/// <summary>
/// Allows a std::vector to be read through a std::istream.
/// </summary>
class compound_document_istreambuf : public std::streambuf
{
    using int_type = std::streambuf::int_type;

public:
    compound_document_istreambuf(const compound_document_entry &entry, compound_document &document)
        : entry_(entry),
          document_(document),
          sector_writer_(current_sector_),
          position_(0)
    {
    }

    compound_document_istreambuf(const compound_document_istreambuf &) = delete;
    compound_document_istreambuf &operator=(const compound_document_istreambuf &) = delete;

    ~compound_document_istreambuf() override;

private:
    std::streamsize xsgetn(char *c, std::streamsize count) override
    {
        auto bytes_read = std::streamsize(0);

        const auto sector_chain = short_stream() ? document_.ssat_ : document_.sat_;
        const auto chain = document_.follow_chain(entry_.start, sector_chain);
        const auto sector_size = short_stream() ? document_.short_sector_size() : document_.sector_size();
        auto current_sector = chain.at(position_ / sector_size);
        auto remaining = std::min(std::size_t(entry_.size) - position_, std::size_t(count));

        while (remaining)
        {
            if (current_sector_.empty() || chain.at(position_ / sector_size) != current_sector)
            {
                current_sector = chain.at(position_ / sector_size);
                sector_writer_.reset();
                if (short_stream())
                {
                    document_.read_short_sector(current_sector, sector_writer_);
                }
                else
                {
                    document_.read_sector(current_sector, sector_writer_);
                }
            }

            const auto available = std::min(entry_.size - position_,
                sector_size - position_ % sector_size);
            const auto to_read = std::min(available, std::size_t(remaining));

            auto start = current_sector_.begin() + static_cast<std::ptrdiff_t>(position_ % sector_size);
            auto end = start + static_cast<std::ptrdiff_t>(to_read);

            for (auto i = start; i < end; ++i)
            {
                *(c++) = static_cast<char>(*i);
            }

            remaining -= to_read;
            position_ += to_read;
            bytes_read += to_read;
        }

        if (position_ < entry_.size && chain.at(position_ / sector_size) != current_sector)
        {
            current_sector = chain.at(position_ / sector_size);
            sector_writer_.reset();
            if (short_stream())
            {
                document_.read_short_sector(current_sector, sector_writer_);
            }
            else
            {
                document_.read_sector(current_sector, sector_writer_);
            }
        }

        return bytes_read;
    }

    bool short_stream()
    {
        return entry_.size < document_.header_.threshold;
    }

    int_type underflow() override
    {
        if (position_ >= entry_.size)
        {
            return traits_type::eof();
        }

        auto old_position = position_;
        auto result = '\0';
        xsgetn(&result, 1);
        position_ = old_position;

        return result;
    }

    int_type uflow() override
    {
        auto result = underflow();
        ++position_;

        return result;
    }

    std::streamsize showmanyc() override
    {
        if (position_ == entry_.size)
        {
            return static_cast<std::streamsize>(-1);
        }

        return static_cast<std::streamsize>(entry_.size - position_);
    }

    std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode) override
    {
        if (way == std::ios_base::beg)
        {
            position_ = 0;
        }
        else if (way == std::ios_base::end)
        {
            position_ = entry_.size;
        }

        if (off < 0)
        {
            if (static_cast<std::size_t>(-off) > position_)
            {
                position_ = 0;
                return static_cast<std::ptrdiff_t>(-1);
            }
            else
            {
                position_ -= static_cast<std::size_t>(-off);
            }
        }
        else if (off > 0)
        {
            if (static_cast<std::size_t>(off) + position_ > entry_.size)
            {
                position_ = entry_.size;
                return static_cast<std::ptrdiff_t>(-1);
            }
            else
            {
                position_ += static_cast<std::size_t>(off);
            }
        }

        return static_cast<std::ptrdiff_t>(position_);
    }

    std::streampos seekpos(std::streampos sp, std::ios_base::openmode) override
    {
        if (sp < 0)
        {
            position_ = 0;
        }
        else if (static_cast<std::size_t>(sp) > entry_.size)
        {
            position_ = entry_.size;
        }
        else
        {
            position_ = static_cast<std::size_t>(sp);
        }

        return static_cast<std::ptrdiff_t>(position_);
    }

private:
    const compound_document_entry &entry_;
    compound_document &document_;
    std::vector<byte> current_sector_;
    binary_writer<byte> sector_writer_;
    std::size_t position_;
};

compound_document_istreambuf::~compound_document_istreambuf()
{
}

/// <summary>
/// Allows a std::vector to be written through a std::ostream.
/// </summary>
class compound_document_ostreambuf : public std::streambuf
{
    using int_type = std::streambuf::int_type;

public:
    compound_document_ostreambuf(compound_document_entry &entry, compound_document &document)
        : entry_(entry),
          document_(document),
          current_sector_(document.header_.threshold),
          sector_reader_(current_sector_),
          position_(0)
    {
        setp(reinterpret_cast<char *>(current_sector_.data()),
            reinterpret_cast<char *>(current_sector_.data() + current_sector_.size()));
    }

    compound_document_ostreambuf(const compound_document_ostreambuf &) = delete;
    compound_document_ostreambuf &operator=(const compound_document_ostreambuf &) = delete;

    ~compound_document_ostreambuf() override;

private:
    int sync() override
    {
        auto written = static_cast<std::size_t>(pptr() - pbase());

        if (written == std::size_t(0))
        {
            return 0;
        }

        sector_reader_.reset();

        if (short_stream())
        {
            if (position_ + written >= document_.header_.threshold)
            {
                convert_to_long_stream();
            }
            else
            {
                if (entry_.start < 0)
                {
                    auto num_sectors = (position_ + written + document_.short_sector_size() - 1) / document_.short_sector_size();
                    chain_ = document_.allocate_short_sectors(num_sectors);
                    entry_.start = chain_.at(0);
                }

                for (auto link : chain_)
                {
                    document_.write_short_sector(sector_reader_, link);
                    sector_reader_.offset(sector_reader_.offset() + document_.short_sector_size());
                }
            }
        }
        else
        {
            const auto sector_index = position_ / document_.sector_size();
            document_.write_sector(sector_reader_, chain_.at(sector_index));
        }

        position_ += written;
        entry_.size = std::max(entry_.size, static_cast<std::uint64_t>(position_));
        document_.write_directory();

        std::fill(current_sector_.begin(), current_sector_.end(), byte(0));
        setp(reinterpret_cast<char *>(current_sector_.data()),
            reinterpret_cast<char *>(current_sector_.data() + current_sector_.size()));

        return 0;
    }

    bool short_stream()
    {
        return entry_.size < document_.header_.threshold;
    }

    int_type overflow(int_type c = traits_type::eof()) override
    {
        sync();

        if (short_stream())
        {
            auto next_sector = document_.allocate_short_sector();
            document_.ssat_.at(static_cast<std::size_t>(back_safe(chain_))) = next_sector;
            chain_.push_back(next_sector);
            document_.write_ssat();
        }
        else
        {
            auto next_sector = document_.allocate_sector();
            document_.sat_.at(static_cast<std::size_t>(back_safe(chain_))) = next_sector;
            chain_.push_back(next_sector);
            document_.write_sat();
        }

        auto value = static_cast<std::uint8_t>(c);

        if (c != traits_type::eof())
        {
            current_sector_.at(position_ % current_sector_.size()) = value;
        }

        pbump(1);

        return traits_type::to_int_type(static_cast<char>(value));
    }

    void convert_to_long_stream()
    {
        sector_reader_.reset();

        auto num_sectors = current_sector_.size() / document_.sector_size();
        auto new_chain = document_.allocate_sectors(num_sectors);

        for (auto link : new_chain)
        {
            document_.write_sector(sector_reader_, link);
            sector_reader_.offset(sector_reader_.offset() + document_.short_sector_size());
        }

        current_sector_.resize(document_.sector_size(), 0);
        std::fill(current_sector_.begin(), current_sector_.end(), byte(0));

        if (entry_.start < 0)
        {
            // TODO: deallocate short sectors here
            if (document_.header_.num_short_sectors == 0)
            {
                document_.entries_.at(0).start = EndOfChain;
            }
        }

        chain_ = new_chain;
        entry_.start = chain_.at(0);
        document_.write_directory();
    }

    std::streampos seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode) override
    {
        if (way == std::ios_base::beg)
        {
            position_ = 0;
        }
        else if (way == std::ios_base::end)
        {
            position_ = entry_.size;
        }

        if (off < 0)
        {
            if (static_cast<std::size_t>(-off) > position_)
            {
                position_ = 0;
                return static_cast<std::ptrdiff_t>(-1);
            }
            else
            {
                position_ -= static_cast<std::size_t>(-off);
            }
        }
        else if (off > 0)
        {
            if (static_cast<std::size_t>(off) + position_ > entry_.size)
            {
                position_ = entry_.size;
                return static_cast<std::ptrdiff_t>(-1);
            }
            else
            {
                position_ += static_cast<std::size_t>(off);
            }
        }

        return static_cast<std::ptrdiff_t>(position_);
    }

    std::streampos seekpos(std::streampos sp, std::ios_base::openmode) override
    {
        if (sp < 0)
        {
            position_ = 0;
        }
        else if (static_cast<std::size_t>(sp) > entry_.size)
        {
            position_ = entry_.size;
        }
        else
        {
            position_ = static_cast<std::size_t>(sp);
        }

        return static_cast<std::ptrdiff_t>(position_);
    }

private:
    compound_document_entry &entry_;
    compound_document &document_;
    std::vector<byte> current_sector_;
    binary_reader<byte> sector_reader_;
    std::size_t position_;
    sector_chain chain_;
};

compound_document_ostreambuf::~compound_document_ostreambuf()
{
    sync();
}

compound_document::compound_document(std::ostream &out)
    : out_(&out),
      stream_in_(nullptr),
      stream_out_(nullptr)
{
    header_.msat.fill(FreeSector);
    write_header();
    insert_entry("/Root Entry", compound_document_entry::entry_type::RootStorage);
}

compound_document::compound_document(std::istream &in)
    : in_(&in),
      stream_in_(nullptr),
      stream_out_(nullptr)
{
    read_header();
    read_msat();
    read_sat();
    read_ssat();
    read_directory();
}

compound_document::~compound_document()
{
    close();
}

void compound_document::close()
{
    stream_out_buffer_.reset(nullptr);
}

std::size_t compound_document::sector_size()
{
    return static_cast<std::size_t>(1) << header_.sector_size_power;
}

std::size_t compound_document::short_sector_size()
{
    return static_cast<std::size_t>(1) << header_.short_sector_size_power;
}

std::istream &compound_document::open_read_stream(const std::string &name)
{
    if (!contains_entry(name, compound_document_entry::entry_type::UserStream))
    {
        throw xlnt::invalid_file("compound document entry of type UserStream not found at path: " + name);
    }

    const auto entry_id = find_entry(name, compound_document_entry::entry_type::UserStream);
    const auto &entry = entries_.at(static_cast<std::size_t>(entry_id));

    stream_in_buffer_.reset(new compound_document_istreambuf(entry, *this));
    stream_in_.rdbuf(stream_in_buffer_.get());

    return stream_in_;
}

std::ostream &compound_document::open_write_stream(const std::string &name)
{
    auto entry_id = contains_entry(name, compound_document_entry::entry_type::UserStream)
        ? find_entry(name, compound_document_entry::entry_type::UserStream)
        : insert_entry(name, compound_document_entry::entry_type::UserStream);
    auto &entry = entries_.at(static_cast<std::size_t>(entry_id));

    stream_out_buffer_.reset(new compound_document_ostreambuf(entry, *this));
    stream_out_.rdbuf(stream_out_buffer_.get());

    return stream_out_;
}

template <typename T>
void compound_document::write_sector(binary_reader<T> &reader, sector_id id)
{
    out_->seekp(static_cast<std::ptrdiff_t>(sector_data_start() + sector_size() * static_cast<std::size_t>(id)));
    out_->write(reinterpret_cast<const char *>(reader.data() + reader.offset()),
        static_cast<std::ptrdiff_t>(std::min(sector_size(), reader.bytes() - reader.offset())));
}

template <typename T>
void compound_document::write_short_sector(binary_reader<T> &reader, sector_id id)
{
    auto chain = follow_chain(entries_.at(0).start, sat_);
    auto sector_id = chain.at(static_cast<std::size_t>(id) / (sector_size() / short_sector_size()));
    auto sector_offset = static_cast<std::size_t>(id) % (sector_size() / short_sector_size()) * short_sector_size();
    out_->seekp(static_cast<std::ptrdiff_t>(sector_data_start() + sector_size() * static_cast<std::size_t>(sector_id) + sector_offset));
    out_->write(reinterpret_cast<const char *>(reader.data() + reader.offset()),
        static_cast<std::ptrdiff_t>(std::min(short_sector_size(), reader.bytes() - reader.offset())));
}

template <typename T>
void compound_document::read_sector(sector_id id, binary_writer<T> &writer)
{
    in_->seekg(static_cast<std::ptrdiff_t>(sector_data_start() + sector_size() * static_cast<std::size_t>(id)));
    std::vector<byte> sector(sector_size(), 0);
    in_->read(reinterpret_cast<char *>(sector.data()), static_cast<std::ptrdiff_t>(sector_size()));
    writer.append(sector);
}

template <typename T>
void compound_document::read_sector_chain(sector_id start, binary_writer<T> &writer)
{
    for (auto link : follow_chain(start, sat_))
    {
        read_sector(link, writer);
    }
}

template <typename T>
void compound_document::read_sector_chain(sector_id start, binary_writer<T> &writer, sector_id offset, std::size_t count)
{
    auto chain = follow_chain(start, sat_);

    for (auto i = std::size_t(0); i < count; ++i)
    {
        read_sector(chain.at(offset + i), writer);
    }
}

template <typename T>
void compound_document::read_short_sector(sector_id id, binary_writer<T> &writer)
{
    const auto container_chain = follow_chain(entries_.at(0).start, sat_);
    auto container = std::vector<byte>();
    auto container_writer = binary_writer<byte>(container);

    for (auto sector : container_chain)
    {
        read_sector(sector, container_writer);
    }

    auto container_reader = binary_reader<byte>(container);
    container_reader.offset(static_cast<std::size_t>(id) * short_sector_size());

    writer.append(container_reader, short_sector_size());
}

template <typename T>
void compound_document::read_short_sector_chain(sector_id start, binary_writer<T> &writer)
{
    for (auto link : follow_chain(start, ssat_))
    {
        read_short_sector(link, writer);
    }
}

template <typename T>
void compound_document::read_short_sector_chain(sector_id start, binary_writer<T> &writer, sector_id offset, std::size_t count)
{
    auto chain = follow_chain(start, ssat_);

    for (auto i = std::size_t(0); i < count; ++i)
    {
        read_short_sector(chain.at(offset + i), writer);
    }
}

sector_id compound_document::allocate_sector()
{
    const auto sectors_per_sector = sector_size() / sizeof(sector_id);
    auto next_free_iter = std::find(sat_.begin(), sat_.end(), FreeSector);

    if (next_free_iter == sat_.end())
    {
        auto next_msat_index = header_.num_msat_sectors;
        auto new_sat_sector_id = sector_id(sat_.size());

        msat_.push_back(new_sat_sector_id);
        write_msat();

        header_.msat.at(msat_.size() - 1) = new_sat_sector_id;
        ++header_.num_msat_sectors;
        write_header();

        sat_.resize(sat_.size() + sectors_per_sector, FreeSector);
        sat_.at(static_cast<std::size_t>(new_sat_sector_id)) = SATSector;

        auto sat_reader = binary_reader<sector_id>(sat_);
        sat_reader.offset(next_msat_index * sectors_per_sector);
        write_sector(sat_reader, new_sat_sector_id);

        next_free_iter = std::find(sat_.begin(), sat_.end(), FreeSector);
    }

    auto next_free = sector_id(next_free_iter - sat_.begin());
    sat_.at(static_cast<std::size_t>(next_free)) = EndOfChain;

    write_sat();

    auto empty_sector = std::vector<byte>(sector_size());
    auto empty_sector_reader = binary_reader<byte>(empty_sector);
    write_sector(empty_sector_reader, next_free);

    return next_free;
}

sector_chain compound_document::allocate_sectors(std::size_t count)
{
    if (count == std::size_t(0)) return {};

    auto chain = sector_chain();
    auto current = allocate_sector();

    for (auto i = std::size_t(1); i < count; ++i)
    {
        chain.push_back(current);
        auto next = allocate_sector();
        sat_.at(static_cast<std::size_t>(current)) = next;
        current = next;
    }

    chain.push_back(current);
    write_sat();

    return chain;
}

sector_chain compound_document::follow_chain(sector_id start, const sector_chain &table)
{
    auto chain = sector_chain();
    auto current = start;

    while (current >= 0)
    {
        chain.push_back(current);
        current = table.at(static_cast<std::size_t>(current));
    }

    return chain;
}

sector_chain compound_document::allocate_short_sectors(std::size_t count)
{
    if (count == std::size_t(0)) return {};

    auto chain = sector_chain();
    auto current = allocate_short_sector();

    for (auto i = std::size_t(1); i < count; ++i)
    {
        chain.push_back(current);
        auto next = allocate_short_sector();
        ssat_.at(static_cast<std::size_t>(current)) = next;
        current = next;
    }

    chain.push_back(current);
    write_ssat();

    return chain;
}

sector_id compound_document::allocate_short_sector()
{
    const auto sectors_per_sector = sector_size() / sizeof(sector_id);
    auto next_free_iter = std::find(ssat_.begin(), ssat_.end(), FreeSector);

    if (next_free_iter == ssat_.end())
    {
        auto new_ssat_sector_id = allocate_sector();

        if (header_.ssat_start < 0)
        {
            header_.ssat_start = new_ssat_sector_id;
        }
        else
        {
            auto ssat_chain = follow_chain(header_.ssat_start, sat_);
            sat_.at(static_cast<std::size_t>(back_safe(ssat_chain))) = new_ssat_sector_id;
            write_sat();
        }

        write_header();

        auto old_size = ssat_.size();
        ssat_.resize(old_size + sectors_per_sector, FreeSector);

        auto ssat_reader = binary_reader<sector_id>(ssat_);
        ssat_reader.offset(old_size / sectors_per_sector);
        write_sector(ssat_reader, new_ssat_sector_id);

        next_free_iter = std::find(ssat_.begin(), ssat_.end(), FreeSector);
    }

    ++header_.num_short_sectors;
    write_header();

    auto next_free = sector_id(next_free_iter - ssat_.begin());
    ssat_.at(static_cast<std::size_t>(next_free)) = EndOfChain;

    write_ssat();

    const auto short_sectors_per_sector = sector_size() / short_sector_size();
    const auto required_container_sectors = static_cast<std::size_t>(next_free) / short_sectors_per_sector + std::size_t(1);

    if (required_container_sectors > 0)
    {
        if (entries_.at(0).start < 0)
        {
            entries_.at(0).start = allocate_sector();
            write_entry(0);
        }

        auto container_chain = follow_chain(entries_.at(0).start, sat_);

        if (required_container_sectors > container_chain.size())
        {
            sat_.at(static_cast<std::size_t>(back_safe(container_chain))) = allocate_sector();
            write_sat();
        }
    }

    return next_free;
}

directory_id compound_document::next_empty_entry()
{
    auto entry_id = directory_id(0);

    for (; entry_id < directory_id(entries_.size()); ++entry_id)
    {
        auto &entry = entries_.at(static_cast<std::size_t>(entry_id));

        if (entry.type == compound_document_entry::entry_type::Empty)
        {
            return entry_id;
        }
    }

    // entry_id is now equal to entries_.size()

    if (header_.directory_start < 0)
    {
        header_.directory_start = allocate_sector();
    }
    else
    {
        auto directory_chain = follow_chain(header_.directory_start, sat_);
        sat_.at(static_cast<std::size_t>(back_safe(directory_chain))) = allocate_sector();
        write_sat();
    }

    const auto entries_per_sector = sector_size()
        / sizeof(compound_document_entry);

    for (auto i = std::size_t(0); i < entries_per_sector; ++i)
    {
        entries_.emplace_back();
        entries_.back().type = compound_document_entry::entry_type::Empty;
        write_entry(entry_id + directory_id(i));
    }

    return entry_id;
}

directory_id compound_document::insert_entry(
    const std::string &name,
    compound_document_entry::entry_type type)
{
    auto entry_id = next_empty_entry();
    auto &entry = entries_.at(static_cast<std::size_t>(entry_id));

    auto parent_id = directory_id(0);
    auto split = split_path(name);
    auto filename = back_safe(split);
    split.pop_back();

    if (split.size() > 1)
    {
        auto joined_path = join_path(split);
        parent_id = find_entry(joined_path, compound_document_entry::entry_type::UserStorage);

        if (parent_id < 0)
        {
            throw xlnt::key_not_found("parent compound document entry of type UserStorage not found at path \"" + joined_path + "\", "
                "necessary to insert entry \"" + name + "\" of type " + std::to_string(static_cast<int>(type)));
        }

        parent_storage_[entry_id] = parent_id;
    }

    entry.name(filename);
    entry.type = type;

    tree_insert(entry_id, parent_id);
    write_directory();

    return entry_id;
}

std::size_t compound_document::sector_data_start()
{
    return sizeof(compound_document_header);
}

bool compound_document::contains_entry(const std::string &path,
    compound_document_entry::entry_type type)
{
    return find_entry(path, type) >= 0;
}

directory_id compound_document::find_entry(const std::string &name,
    compound_document_entry::entry_type type)
{
    if (type == compound_document_entry::entry_type::RootStorage
        && (name == "/" || name == "/Root Entry")) return 0;

    auto entry_id = directory_id(0);

    for (auto &entry : entries_)
    {
        if (entry.type == type && tree_path(entry_id) == name)
        {
            return entry_id;
        }

        ++entry_id;
    }

    return End;
}

void compound_document::print_directory()
{
    auto entry_id = directory_id(0);

    for (auto &entry : entries_)
    {
        if (entry.type == compound_document_entry::entry_type::UserStream)
        {
            std::cout << tree_path(entry_id) << std::endl;
        }

        ++entry_id;
    }
}

void compound_document::write_directory()
{
    for (auto entry_id = std::size_t(0); entry_id < entries_.size(); ++entry_id)
    {
        write_entry(directory_id(entry_id));
    }
}

void compound_document::read_directory()
{
    const auto entries_per_sector = sector_size() / sizeof(compound_document_entry);
    const auto num_entries = follow_chain(header_.directory_start, sat_).size() * entries_per_sector;

    for (auto entry_id = std::size_t(0); entry_id < num_entries; ++entry_id)
    {
        entries_.emplace_back();
        read_entry(directory_id(entry_id));
    }

    auto stack = std::vector<directory_id>();
    auto storage_siblings = std::vector<directory_id>();
    auto stream_siblings = std::vector<directory_id>();

    auto directory_stack = std::vector<directory_id>();
    directory_stack.push_back(directory_id(0));

    while (!directory_stack.empty())
    {
        auto current_storage_id = directory_stack.back();
        directory_stack.pop_back();

        if (tree_child(current_storage_id) < 0) continue;

        auto storage_stack = std::vector<directory_id>();
        auto storage_root_id = tree_child(current_storage_id);
        parent_[storage_root_id] = End;
        storage_stack.push_back(storage_root_id);

        while (!storage_stack.empty())
        {
            auto current_entry_id = storage_stack.back();
            auto current_entry = entries_.at(static_cast<std::size_t>(current_entry_id));
            storage_stack.pop_back();

            parent_storage_[current_entry_id] = current_storage_id;

            if (current_entry.type == compound_document_entry::entry_type::UserStorage)
            {
                directory_stack.push_back(current_entry_id);
            }

            if (tree_left(current_entry_id) >= 0)
            {
                storage_stack.push_back(tree_left(current_entry_id));
                tree_parent(tree_left(current_entry_id)) = current_entry_id;
            }

            if (tree_right(current_entry_id) >= 0)
            {
                storage_stack.push_back(tree_right(current_entry_id));
                tree_parent(tree_right(current_entry_id)) = current_entry_id;
            }
        }
    }
}

void compound_document::tree_insert(directory_id new_id, directory_id storage_id)
{
    using entry_color = compound_document_entry::entry_color;

    parent_storage_[new_id] = storage_id;

    tree_left(new_id) = End;
    tree_right(new_id) = End;

    if (tree_root(new_id) == End)
    {
        if (new_id != 0)
        {
            tree_root(new_id) = new_id;
        }

        tree_color(new_id) = entry_color::Black;
        tree_parent(new_id) = End;

        return;
    }

    // normal tree insert
    // (will probably unbalance the tree, fix after)
    auto x = tree_root(new_id);
    auto y = End;

    while (x >= 0)
    {
        y = x;

        if (compare_keys(tree_key(new_id), tree_key(x)) > 0)
        {
            x = tree_right(x);
        }
        else
        {
            x = tree_left(x);
        }
    }

    tree_parent(new_id) = y;

    if (compare_keys(tree_key(new_id), tree_key(y)) > 0)
    {
        tree_right(y) = new_id;
    }
    else
    {
        tree_left(y) = new_id;
    }

    tree_insert_fixup(new_id);
}

std::string compound_document::tree_path(directory_id id)
{
    auto storage_id = parent_storage_[id];
    auto result = std::vector<std::string>();

    while (storage_id > 0)
    {
        storage_id = parent_storage_[storage_id];
        result.emplace_back(entries_.at(static_cast<std::size_t>(storage_id)).name());
    }

    return "/" + join_path(result) + entries_.at(static_cast<std::size_t>(id)).name();
}

void compound_document::tree_rotate_left(directory_id x)
{
    auto y = tree_right(x);

    // turn y's left subtree into x's right subtree
    tree_right(x) = tree_left(y);

    if (tree_left(y) != End)
    {
        tree_parent(tree_left(y)) = x;
    }

    // link x's parent to y
    tree_parent(y) = tree_parent(x);

    if (tree_parent(x) == End)
    {
        tree_root(x) = y;
    }
    else if (x == tree_left(tree_parent(x)))
    {
        tree_left(tree_parent(x)) = y;
    }
    else
    {
        tree_right(tree_parent(x)) = y;
    }

    // put x on y's left
    tree_left(y) = x;
    tree_parent(x) = y;
}

void compound_document::tree_rotate_right(directory_id y)
{
    auto x = tree_left(y);

    // turn x's right subtree into y's left subtree
    tree_left(y) = tree_right(x);

    if (tree_right(x) != End)
    {
        tree_parent(tree_right(x)) = y;
    }

    // link y's parent to x
    tree_parent(x) = tree_parent(y);

    if (tree_parent(y) == End)
    {
        tree_root(y) = x;
    }
    else if (y == tree_left(tree_parent(y)))
    {
        tree_left(tree_parent(y)) = x;
    }
    else
    {
        tree_right(tree_parent(y)) = x;
    }

    // put y on x's right
    tree_right(x) = y;
    tree_parent(y) = x;
}

void compound_document::tree_insert_fixup(directory_id x)
{
    using entry_color = compound_document_entry::entry_color;

    tree_color(x) = entry_color::Red;

    while (x != tree_root(x) && tree_color(tree_parent(x)) == entry_color::Red)
    {
        if (tree_parent(x) == tree_left(tree_parent(tree_parent(x))))
        {
            auto y = tree_right(tree_parent(tree_parent(x)));

            if (y >= 0 && tree_color(y) == entry_color::Red)
            {
                // case 1
                tree_color(tree_parent(x)) = entry_color::Black;
                tree_color(y) = entry_color::Black;
                tree_color(tree_parent(tree_parent(x))) = entry_color::Red;
                x = tree_parent(tree_parent(x));
            }
            else
            {
                if (x == tree_right(tree_parent(x)))
                {
                    // case 2
                    x = tree_parent(x);
                    tree_rotate_left(x);
                }

                // case 3
                tree_color(tree_parent(x)) = entry_color::Black;
                tree_color(tree_parent(tree_parent(x))) = entry_color::Red;
                tree_rotate_right(tree_parent(tree_parent(x)));
            }
        }
        else // same as above with left and right switched
        {
            auto y = tree_left(tree_parent(tree_parent(x)));

            if (y >= 0 && tree_color(y) == entry_color::Red)
            {
                //case 1
                tree_color(tree_parent(x)) = entry_color::Black;
                tree_color(y) = entry_color::Black;
                tree_color(tree_parent(tree_parent(x))) = entry_color::Red;
                x = tree_parent(tree_parent(x));
            }
            else
            {
                if (x == tree_left(tree_parent(x)))
                {
                    // case 2
                    x = tree_parent(x);
                    tree_rotate_right(x);
                }

                // case 3
                tree_color(tree_parent(x)) = entry_color::Black;
                tree_color(tree_parent(tree_parent(x))) = entry_color::Red;
                tree_rotate_left(tree_parent(tree_parent(x)));
            }
        }
    }

    tree_color(tree_root(x)) = entry_color::Black;
}

directory_id &compound_document::tree_left(directory_id id)
{
    return entries_.at(static_cast<std::size_t>(id)).prev;
}

directory_id &compound_document::tree_right(directory_id id)
{
    return entries_.at(static_cast<std::size_t>(id)).next;
}

directory_id &compound_document::tree_parent(directory_id id)
{
    return parent_[id];
}

directory_id &compound_document::tree_root(directory_id id)
{
    return tree_child(parent_storage_[id]);
}

directory_id &compound_document::tree_child(directory_id id)
{
    return entries_.at(static_cast<std::size_t>(id)).child;
}

std::string compound_document::tree_key(directory_id id)
{
    return entries_.at(static_cast<std::size_t>(id)).name();
}

compound_document_entry::entry_color &compound_document::tree_color(directory_id id)
{
    return entries_.at(static_cast<std::size_t>(id)).color;
}

void compound_document::read_header()
{
    in_->seekg(0, std::ios::beg);
    in_->read(reinterpret_cast<char *>(&header_), sizeof(compound_document_header));

    // Header Signature (8 bytes): Identification signature for the compound file structure, and MUST be
    // set to the value 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1.
    if (header_.header_signature != 0xE11AB1A1E011CFD0)
    {
        throw xlnt::invalid_file("invalid header signature, expected 0xE11AB1A1E011CFD0 but got " + format_hex(header_.header_signature));
    }

    // Header CLSID (16 bytes): Reserved and unused class ID that MUST be set to all zeroes (CLSID_NULL).
    if (std::any_of(header_.header_clsid.begin(), header_.header_clsid.end(), [](auto i) { return i != 0; }))
    {
        std::string exception_str = "invalid header CLSID, expected only zeros but got: ";
        for (auto val : header_.header_clsid)
        {
            exception_str += fmt::format("{:02x} ", val);
        }
        throw xlnt::invalid_file(exception_str);
    }

    // Major Version (2 bytes): Version number for breaking changes. This field MUST be set to either
    // 0x0003 (version 3) or 0x0004 (version 4).
    if (header_.major_version != 3 && header_.major_version != 4)
    {
        throw xlnt::invalid_file("invalid major version, expected 3 or 4 but got " + std::to_string(header_.major_version));
    }

    // Byte Order (2 bytes): This field MUST be set to 0xFFFE. This field is a byte order mark for all integer
    // fields, specifying little-endian byte order.
    if (static_cast<std::uint16_t>(header_.byte_order) != 0xFFFE)
    {
        throw xlnt::invalid_file("invalid byte order, expected 0xFFFE (little-endian) but got " +
            fmt::format("0x{:04X}", static_cast<std::uint16_t>(header_.byte_order)));
    }

    // Sector Shift (2 bytes): This field MUST be set to 0x0009, or 0x000c, depending on the Major
    // Version field. This field specifies the sector size of the compound file as a power of 2.
    // - If Major Version is 3, the Sector Shift MUST be 0x0009, specifying a sector size of 512 bytes.
    // - If Major Version is 4, the Sector Shift MUST be 0x000C, specifying a sector size of 4096 bytes.
    if (!((header_.major_version == 3 && header_.sector_size_power == 0x0009) ||
        (header_.major_version == 4 && header_.sector_size_power == 0x000C)))
    {
        throw xlnt::invalid_file("invalid combination of sector size power and major version, got sector_size_power = " +
            fmt::format("0x{:04X}", header_.sector_size_power) + "; major_version = " + std::to_string(header_.major_version));
    }

    // Mini Sector Shift (2 bytes): This field MUST be set to 0x0006. This field specifies the sector size of
    // the Mini Stream as a power of 2. The sector size of the Mini Stream MUST be 64 bytes.
    if (header_.short_sector_size_power != 0x0006)
    {
        throw xlnt::invalid_file("invalid short sector size power, expected 0x0006 but got " + fmt::format("0x{:04X}", header_.short_sector_size_power));
    }

    // Reserved (6 bytes): This field MUST be set to all zeroes.
    if (std::any_of(header_.reserved.begin(), header_.reserved.end(), [](auto i) { return i != 0; }))
    {
        std::string exception_str = "invalid reserved field, expected only zeros but got: ";
        for (auto val : header_.reserved)
        {
            exception_str += fmt::format("{:02x} ", val);
        }
        throw xlnt::invalid_file(exception_str);
    }

    // Number of Directory Sectors (4 bytes): This integer field contains the count of the number of
    // directory sectors in the compound file.
    // - If Major Version is 3, the Number of Directory Sectors MUST be zero. This field is not
    //   supported for version 3 compound files.
    if (header_.major_version == 3 && header_.num_directory_sectors != 0)
    {
        throw xlnt::invalid_file("invalid number of directory sectors for major version 3: expected 0 directory sectors but got " +
            std::to_string(header_.num_directory_sectors));
    }

    // Mini Stream Cutoff Size (4 bytes): This integer field MUST be set to 0x00001000. This field
    // specifies the maximum size of a user-defined data stream that is allocated from the mini FAT
    // and mini stream, and that cutoff is 4,096 bytes. Any user-defined data stream that is greater than
    // or equal to this cutoff size must be allocated as normal sectors from the FAT.
    if (header_.threshold != 0x00001000)
    {
        throw xlnt::invalid_file("invalid mini stream cutoff size, expected 0x00001000 but got " + format_hex(header_.threshold));
    }

    // DIFAT (436 bytes): This array of 32-bit integer fields contains the first 109 FAT sector locations of
    // the compound file.
    // - For version 4 compound files, the header size (512 bytes) is less than the sector size (4,096
    //   bytes), so the remaining part of the header (3,584 bytes) MUST be filled with all zeroes.
    if (header_.major_version == 4)
    {
        std::array<std::uint8_t, 3584> remaining {{ 0 }};
        in_->read(reinterpret_cast<char *>(remaining.data()), sizeof(remaining));

        if (std::any_of(remaining.begin(), remaining.end(), [](auto i) { return i != 0; }))
        {
            std::string exception_str = "invalid remaining bytes in header (major version 4), expected only zeros but got: ";
            for (auto val : remaining)
            {
                exception_str += fmt::format("{:02x} ", val);
            }
            throw xlnt::invalid_file(exception_str);
        }
    }
}

void compound_document::read_msat()
{
    msat_.clear();

    auto msat_sector = header_.extra_msat_start;
    auto msat_writer = binary_writer<sector_id>(msat_);

    for (auto i = std::uint32_t(0); i < header_.num_msat_sectors; ++i)
    {
        if (i < std::uint32_t(109))
        {
            msat_writer.write(header_.msat.at(i));
        }
        else
        {
            read_sector(msat_sector, msat_writer);

            msat_sector = back_safe(msat_);
            msat_.pop_back();
        }
    }
}

void compound_document::read_sat()
{
    sat_.clear();
    auto sat_writer = binary_writer<sector_id>(sat_);

    for (auto msat_sector : msat_)
    {
        read_sector(msat_sector, sat_writer);
    }
}

void compound_document::read_ssat()
{
    ssat_.clear();
    auto ssat_writer = binary_writer<sector_id>(ssat_);

    for (auto ssat_sector : follow_chain(header_.ssat_start, sat_))
    {
        read_sector(ssat_sector, ssat_writer);
    }
}

std::string format_entry_info(directory_id entry_id, sector_id sector_id)
{
    // The formatted IDs should be as short as possible to keep the exception message readable - so we do not add leading zeros.
    return "(entry " + fmt::format("0x{:X}", entry_id) + " in sector " + fmt::format("0x{:X}", sector_id) + ")";
}

void compound_document::read_entry(directory_id id)
{
    const auto directory_chain = follow_chain(header_.directory_start, sat_);
    const auto entries_per_sector = sector_size() / sizeof(compound_document_entry);
    const auto directory_sector = directory_chain.at(static_cast<std::size_t>(id) / entries_per_sector);
    const auto offset = sector_size() * static_cast<std::size_t>(directory_sector)
        + ((static_cast<std::size_t>(id) % entries_per_sector) * sizeof(compound_document_entry));

    in_->seekg(static_cast<std::ptrdiff_t>(sector_data_start() + offset), std::ios::beg);
    compound_document_entry &current_entry = entries_.at(static_cast<std::size_t>(id));
    in_->read(reinterpret_cast<char *>(&current_entry), sizeof(compound_document_entry));

    // First check the length, as we'll need this for the string itself.
    // Directory Entry Name Length (2 bytes): This field MUST match the length of the Directory Entry
    // Name Unicode string in bytes. The length MUST be a multiple of 2 and include the terminating null
    // character in the count. This length MUST NOT exceed 64, the maximum size of the Directory Entry
    // Name field.
    if (current_entry.name_length < 2 || current_entry.name_length > 64)
    {
        throw xlnt::invalid_file("invalid entry name length " + format_entry_info(id, directory_sector) +
            ", expected >= 2 and <= 64, but got " + std::to_string(current_entry.name_length));
    }
    else if (current_entry.name_length % 2 != 0)
    {
        throw xlnt::invalid_file("invalid entry name length " + format_entry_info(id, directory_sector) +
            ", which must be a multiple of 2, but got " + std::to_string(current_entry.name_length));
    }

    // Directory Entry Name (64 bytes): This field MUST contain a Unicode string for the storage or
    // stream name encoded in UTF-16. The name MUST be terminated with a UTF-16 terminating null
    // character. Thus, storage and stream names are limited to 32 UTF-16 code points, including the
    // terminating null character. When locating an object in the compound file except for the root
    // storage, the directory entry name is compared by using a special case-insensitive uppercase
    // mapping, described in Red-Black Tree. The following characters are illegal and MUST NOT be part
    // of the name: '/', '\', ':', '!'.
    std::uint16_t name_length_characters = (current_entry.name_length / 2) - 1; // does NOT include \0 at the end
    if (current_entry.name_array.at(name_length_characters) != u'\0')
    {
        std::string exception_str = "invalid entry name " + format_entry_info(id, directory_sector) +
            ", which must be terminated with \\0 but is terminated with " +
            fmt::format("0x{:04X}", static_cast<uint16_t>(current_entry.name_array.at(name_length_characters))) +
            "\nString has a length of " + std::to_string(name_length_characters) + " characters (" +
            std::to_string(current_entry.name_length) + " bytes including \\0). Full buffer contents:\n";
        for (auto val : current_entry.name_array)
        {
            exception_str += fmt::format("{:04x} ", static_cast<uint16_t>(val));
        }

        throw xlnt::invalid_file(exception_str);
    }

    for (std::uint16_t n = 0; n < name_length_characters; ++n)
    {
        auto curr = current_entry.name_array.at(n);
        if (curr == u'/' || curr == u'\\' || curr == u':' || curr == u'!')
        {
            throw xlnt::invalid_file("invalid entry name " + format_entry_info(id, directory_sector) +
                ", which contains invalid character " +
                fmt::format("0x{:04X}", static_cast<uint16_t>(curr)) + " at position " + std::to_string(n));
        }
    }

    // Object Type (1 byte): This field MUST be 0x00, 0x01, 0x02, or 0x05, depending on the actual type
    // of object. All other values are not valid.
    if (static_cast<uint8_t>(current_entry.type) != 0x00 && // Empty
        static_cast<uint8_t>(current_entry.type) != 0x01 && // UserStorage
        static_cast<uint8_t>(current_entry.type) != 0x02 && // UserStream
        static_cast<uint8_t>(current_entry.type) != 0x05) // RootStorage
    {
        throw xlnt::invalid_file("invalid entry object type " + format_entry_info(id, directory_sector) +
            ", expected 0, 1, 2 or 5 but got " + std::to_string(static_cast<int>(current_entry.type)));
    }

    // Color Flag (1 byte): This field MUST be 0x00 (red) or 0x01 (black). All other values are not valid.
    if (static_cast<uint8_t>(current_entry.color) != 0 && static_cast<uint8_t>(current_entry.color) != 1)
    {
        throw xlnt::invalid_file("invalid entry color " + format_entry_info(id, directory_sector) +
            ", expected 0 or 1, but got " + std::to_string(static_cast<int>(current_entry.color)));
    }

    // CLSID (16 bytes): This field contains an object class GUID, if this entry is for a storage object or
    // root storage object. For a stream object, this field MUST be set to all zeroes. A value containing all
    // zeroes in a storage or root storage directory entry is valid, and indicates that no object class is
    // associated with the storage. If an implementation of the file format enables applications to create
    // storage objects without explicitly setting an object class GUID, it MUST write all zeroes by default.
    // If this value is not all zeroes, the object class GUID can be used as a parameter to start
    // applications.
    if (current_entry.type == compound_document_entry::entry_type::UserStream &&
        std::any_of(current_entry.clsid.begin(), current_entry.clsid.end(), [](auto i) { return i != 0; }))
    {
        std::string exception_str = "invalid entry CLSID " + format_entry_info(id, directory_sector) +
            " for UserStream type, espected all zeros but got: ";
        for (auto val : current_entry.clsid)
        {
            exception_str += fmt::format("{:02x} ", val);
        }
        throw xlnt::invalid_file(exception_str);
    }

    // Creation Time (8 bytes): This field contains the creation time for a storage object, or all zeroes to
    // indicate that the creation time of the storage object was not recorded. The Windows FILETIME
    // structure is used to represent this field in UTC. For a stream object, this field MUST be all zeroes.
    // For a root storage object, this field MUST be all zeroes, and the creation time is retrieved or set on
    // the compound file itself.
    if ((current_entry.type == compound_document_entry::entry_type::UserStream ||
        current_entry.type == compound_document_entry::entry_type::RootStorage) &&
        current_entry.creation_time != 0)
    {
        throw xlnt::invalid_file("invalid entry creation time " + format_entry_info(id, directory_sector) +
            " for type " + std::to_string(static_cast<int>(current_entry.type)) +
            ", expected 0 but got " + std::to_string(current_entry.creation_time));
    }

    // Modified Time (8 bytes): This field contains the modification time for a storage object, or all
    // zeroes to indicate that the modified time of the storage object was not recorded. The Windows
    // FILETIME structure is used to represent this field in UTC. For a stream object, this field MUST be
    // all zeroes. For a root storage object, this field MAY<2> be set to all zeroes, and the modified time
    // is retrieved or set on the compound file itself.
    if (current_entry.type == compound_document_entry::entry_type::UserStream &&
        current_entry.modified_time != 0)
    {
        throw xlnt::invalid_file("invalid entry modification time " + format_entry_info(id, directory_sector) +
            " for type UserStream, expected 0 but got " + std::to_string(current_entry.modified_time));
    }

    // Starting Sector Location (4 bytes): This field contains the first sector location if this is a stream
    // object. For a root storage object, this field MUST contain the first sector of the mini stream, if the
    // mini stream exists. For a storage object, this field MUST be set to all zeroes.
    if (current_entry.type == compound_document_entry::entry_type::UserStorage &&
        current_entry.start != 0)
    {
        throw xlnt::invalid_file("invalid entry start sector location " + format_entry_info(id, directory_sector) +
            " for type UserStorage, expected 0 but got " +
            std::to_string(current_entry.start));
    }

    // Stream Size (8 bytes): ... (see below for the rest)
    // - For a version 3 compound file 512-byte sector size, the value of this field MUST be less than
    //   or equal to 0x80000000. (Equivalently, this requirement can be stated: the size of a stream or
    //   of the mini stream in a version 3 compound file MUST be less than or equal to 2 gigabytes
    //   (GB).) Note that as a consequence of this requirement, the most significant 32 bits of this field
    //   MUST be zero in a version 3 compound file. However, implementers should be aware that
    //   some older implementations did not initialize the most significant 32 bits of this field, and
    //   these bits might therefore be nonzero in files that are otherwise valid version 3 compound
    //   files. Although this document does not normatively specify parser behavior, it is recommended
    //   that parsers ignore the most significant 32 bits of this field in version 3 compound files,
    //   treating it as if its value were zero, unless there is a specific reason to do otherwise (for
    //   example, a parser whose purpose is to verify the correctness of a compound file).
    if (header_.major_version == 3 && current_entry.size > 0x80000000)
    {
        // Note: we have checked above that the only allowed byte order is little-endian.
        current_entry.size = current_entry.size & 0x0000FFFF;
    }

    // Stream Size (8 bytes): This 64-bit integer field contains the size of the user-defined data if this is
    // a stream object. For a root storage object, this field contains the size of the mini stream. For a
    // storage object, this field MUST be set to all zeroes.
    if (current_entry.type == compound_document_entry::entry_type::UserStorage &&
        current_entry.size != 0)
    {
        throw xlnt::invalid_file("invalid entry stream size " + format_entry_info(id, directory_sector) +
            " for type UserStorage, expected 0 but got " +
            std::to_string(current_entry.size));
    }
}

void compound_document::write_header()
{
    out_->seekp(0, std::ios::beg);
    out_->write(reinterpret_cast<char *>(&header_), sizeof(compound_document_header));
}

void compound_document::write_msat()
{
    auto msat_sector = header_.extra_msat_start;

    for (auto i = std::uint32_t(0); i < header_.num_msat_sectors; ++i)
    {
        if (i < std::uint32_t(109))
        {
            header_.msat.at(i) = msat_.at(i);
        }
        else
        {
            auto sector = std::vector<sector_id>();
            auto sector_writer = binary_writer<sector_id>(sector);

            read_sector(msat_sector, sector_writer);

            msat_sector = back_safe(sector);
            sector.pop_back();

            std::copy(sector.begin(), sector.end(), std::back_inserter(msat_));
        }
    }
}

void compound_document::write_sat()
{
    auto sector_reader = binary_reader<sector_id>(sat_);

    for (auto sat_sector : msat_)
    {
        write_sector(sector_reader, sat_sector);
    }
}

void compound_document::write_ssat()
{
    auto sector_reader = binary_reader<sector_id>(ssat_);

    for (auto ssat_sector : follow_chain(header_.ssat_start, sat_))
    {
        write_sector(sector_reader, ssat_sector);
    }
}

void compound_document::write_entry(directory_id id)
{
    const auto directory_chain = follow_chain(header_.directory_start, sat_);
    const auto entries_per_sector = sector_size() / sizeof(compound_document_entry);
    const auto directory_sector = directory_chain.at(static_cast<std::size_t>(id) / entries_per_sector);
    const auto offset = sector_data_start() + sector_size() * static_cast<std::size_t>(directory_sector)
        + ((static_cast<std::size_t>(id) % entries_per_sector) * sizeof(compound_document_entry));

    out_->seekp(static_cast<std::ptrdiff_t>(offset), std::ios::beg);
    out_->write(reinterpret_cast<char *>(&entries_.at(static_cast<std::size_t>(id))), sizeof(compound_document_entry));
}

} // namespace detail
} // namespace xlnt
