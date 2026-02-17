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
            static const std::locale locale;
            std::use_facet<std::ctype<char>>(locale).tolower(&s[0], &s[0] + s.size());

            return s;
        }
    };

    return to_lower(left).compare(to_lower(right));
}

std::vector<std::string> split_path(const std::string &path)
{
    std::vector<std::string> split = std::vector<std::string>();
    std::size_t current = path.find('/');
    std::size_t prev = 0;

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
    std::string joined;

    for (const std::string &part : path)
    {
        joined.append(part);
        joined.push_back('/');
    }

    return joined;
}

template <typename T>
const T & last_elem(const std::vector<T> &vec)
{
    return vec.at(vec.size() - 1);
}

template <typename T>
T & last_elem(std::vector<T> &vec)
{
    return vec.at(vec.size() - 1);
}

} // namespace

namespace xlnt {
namespace detail {

bool is_chain_end(sector_id sector)
{
    expect_valid_sector_or_chain_end(sector);
    return sector == ENDOFCHAIN;
}

bool is_invalid_sector(sector_id sector)
{
    expect_valid_sector_or_chain_end_or_free(sector);
    return sector == ENDOFCHAIN || sector == FREESECT;
}

bool is_invalid_entry(directory_id entry)
{
    expect_valid_entry_or_no_stream(entry);
    return entry == NOSTREAM;
}

void expect_valid_sector_or_chain_end(sector_id sector)
{
    if (sector > MAXREGSECT && sector != ENDOFCHAIN)
    {
        throw xlnt::invalid_parameter("expected valid sector (<= MAXREGSECT, which means <= 0xFFFFFFFA) or ENDOFCHAIN (0xFFFFFFFE)"
            ", but got " + format_hex(sector));
    }
}

void expect_valid_sector_or_chain_end_or_free(sector_id sector)
{
    if (sector > MAXREGSECT && sector != ENDOFCHAIN && sector != FREESECT)
    {
        throw xlnt::invalid_parameter("expected valid sector (<= MAXREGSECT, which means <= 0xFFFFFFFA),"
            " or ENDOFCHAIN (0xFFFFFFFE), or FREESECT (0xFFFFFFFF), but got " + format_hex(sector));
    }
}

void expect_valid_entry_or_no_stream(directory_id entry)
{
    if (entry > MAXREGSID && entry != NOSTREAM)
    {
        throw xlnt::invalid_parameter("expected valid entry (<= MAXREGSID, which means <= 0xFFFFFFFA) or NOSTREAM (0xFFFFFFFF)"
            ", but got " + format_hex(entry));
    }
}

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
          sector_writer_(current_sector_)
    {
    }

    compound_document_istreambuf(const compound_document_istreambuf &) = delete;
    compound_document_istreambuf &operator=(const compound_document_istreambuf &) = delete;

    ~compound_document_istreambuf() override = default;

private:
    std::streamsize xsgetn(char *c, std::streamsize count) override
    {
        std::streamsize bytes_read = 0;

        const sector_chain &sec_chain = short_stream() ? document_.ssat_ : document_.sat_;
        const sector_chain chain = document_.follow_chain(entry_.start, sec_chain);
        const std::uint64_t sector_size = short_stream() ? document_.short_sector_size() : document_.sector_size();
        sector_id current_sector = chain.at(static_cast<std::size_t>(position_ / sector_size));
        std::uint64_t remaining = std::min(entry_.size - position_, static_cast<std::uint64_t>(count));

        while (remaining)
        {
            if (current_sector_.empty() || chain.at(static_cast<std::size_t>(position_ / sector_size)) != current_sector)
            {
                current_sector = chain.at(static_cast<std::size_t>(position_ / sector_size));
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

            const std::uint64_t available = std::min(entry_.size - position_, sector_size - position_ % sector_size);
            const std::uint64_t to_read = std::min(available, remaining);

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

        if (position_ < entry_.size && chain.at(static_cast<std::size_t>(position_ / sector_size)) != current_sector)
        {
            current_sector = chain.at(static_cast<std::size_t>(position_ / sector_size));
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

        std::uint64_t old_position = position_;
        char result = '\0';
        xsgetn(&result, 1);
        position_ = old_position;

        return result;
    }

    int_type uflow() override
    {
        int_type result = underflow();
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
            if (static_cast<std::uint64_t>(-off) > position_)
            {
                position_ = 0;
                return static_cast<std::streamoff>(-1);
            }
            else
            {
                position_ -= static_cast<std::uint64_t>(-off);
            }
        }
        else if (off > 0)
        {
            if (static_cast<std::uint64_t>(off) + position_ > entry_.size)
            {
                position_ = entry_.size;
                return static_cast<std::streamoff>(-1);
            }
            else
            {
                position_ += static_cast<std::uint64_t>(off);
            }
        }

        return static_cast<std::streamoff>(position_);
    }

    std::streampos seekpos(std::streampos sp, std::ios_base::openmode) override
    {
        if (sp < 0)
        {
            position_ = 0;
        }
        else if (static_cast<std::uint64_t>(sp) > entry_.size)
        {
            position_ = entry_.size;
        }
        else
        {
            position_ = static_cast<std::uint64_t>(sp);
        }

        return static_cast<std::streamoff>(position_);
    }

private:
    const compound_document_entry &entry_;
    compound_document &document_;
    std::vector<byte> current_sector_;
    binary_writer<byte> sector_writer_;
    std::uint64_t position_ = 0;
};

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
          sector_reader_(current_sector_)
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
        auto written = static_cast<std::uint64_t>(pptr() - pbase());

        if (written == 0)
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
                if (is_invalid_sector(entry_.start))
                {
                    std::size_t num_sectors = static_cast<std::size_t>(
                        (position_ + written + document_.short_sector_size() - 1) / document_.short_sector_size());
                    chain_ = document_.allocate_short_sectors(num_sectors);
                    entry_.start = chain_.at(0);
                }

                for (sector_id link : chain_)
                {
                    document_.write_short_sector(sector_reader_, link);
                    sector_reader_.offset(sector_reader_.offset() + document_.short_sector_size());
                }
            }
        }
        else
        {
            const std::size_t sector_index = static_cast<std::size_t>(position_ / document_.sector_size());
            document_.write_sector(sector_reader_, chain_.at(sector_index));
        }

        position_ += written;
        entry_.size = std::max(entry_.size, position_);
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
            sector_id next_sector = document_.allocate_short_sector();
            document_.ssat_.at(last_elem(chain_)) = next_sector;
            chain_.push_back(next_sector);
            document_.write_ssat();
        }
        else
        {
            sector_id next_sector = document_.allocate_sector();
            document_.sat_.at(last_elem(chain_)) = next_sector;
            chain_.push_back(next_sector);
            document_.write_sat();
        }

        auto value = static_cast<std::uint8_t>(c);

        if (c != traits_type::eof())
        {
            std::size_t sector_index = static_cast<std::size_t>(position_ % current_sector_.size());
            current_sector_.at(sector_index) = value;
        }

        pbump(1);

        return traits_type::to_int_type(static_cast<char>(value));
    }

    void convert_to_long_stream()
    {
        sector_reader_.reset();

        std::size_t num_sectors = static_cast<std::size_t>(current_sector_.size() / document_.sector_size());
        sector_chain new_chain = document_.allocate_sectors(num_sectors);

        for (sector_id link : new_chain)
        {
            document_.write_sector(sector_reader_, link);
            sector_reader_.offset(sector_reader_.offset() + document_.short_sector_size());
        }

        current_sector_.resize(document_.sector_size(), 0);
        std::fill(current_sector_.begin(), current_sector_.end(), byte(0));

        if (is_invalid_sector(entry_.start))
        {
            // TODO: deallocate short sectors here
            if (document_.header_.num_short_sectors == 0)
            {
                document_.entries_.at(0).start = ENDOFCHAIN;
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
            if (static_cast<std::uint64_t>(-off) > position_)
            {
                position_ = 0;
                return static_cast<std::streamoff>(-1);
            }
            else
            {
                position_ -= static_cast<std::uint64_t>(-off);
            }
        }
        else if (off > 0)
        {
            if (static_cast<std::uint64_t>(off) + position_ > entry_.size)
            {
                position_ = entry_.size;
                return static_cast<std::streamoff>(-1);
            }
            else
            {
                position_ += static_cast<std::uint64_t>(off);
            }
        }

        return static_cast<std::streamoff>(position_);
    }

    std::streampos seekpos(std::streampos sp, std::ios_base::openmode) override
    {
        if (sp < 0)
        {
            position_ = 0;
        }
        else if (static_cast<std::uint64_t>(sp) > entry_.size)
        {
            position_ = entry_.size;
        }
        else
        {
            position_ = static_cast<std::uint64_t>(sp);
        }

        return static_cast<std::streamoff>(position_);
    }

private:
    compound_document_entry &entry_;
    compound_document &document_;
    std::vector<byte> current_sector_;
    binary_reader<byte> sector_reader_;
    std::uint64_t position_ = 0;
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

std::uint64_t compound_document::sector_size()
{
    return static_cast<std::uint64_t>(1) << header_.sector_size_power;
}

std::uint64_t compound_document::short_sector_size()
{
    return static_cast<std::uint64_t>(1) << header_.short_sector_size_power;
}

std::istream &compound_document::open_read_stream(const std::string &name)
{
    if (!contains_entry(name, compound_document_entry::entry_type::UserStream))
    {
        throw xlnt::invalid_file("compound document entry of type UserStream not found at path: " + name);
    }

    const directory_id entry_id = find_entry(name, compound_document_entry::entry_type::UserStream);
    const compound_document_entry &entry = entries_.at(entry_id);

    stream_in_buffer_.reset(new compound_document_istreambuf(entry, *this));
    stream_in_.rdbuf(stream_in_buffer_.get());

    return stream_in_;
}

std::ostream &compound_document::open_write_stream(const std::string &name)
{
    directory_id entry_id = contains_entry(name, compound_document_entry::entry_type::UserStream)
        ? find_entry(name, compound_document_entry::entry_type::UserStream)
        : insert_entry(name, compound_document_entry::entry_type::UserStream);
    compound_document_entry &entry = entries_.at(entry_id);

    stream_out_buffer_.reset(new compound_document_ostreambuf(entry, *this));
    stream_out_.rdbuf(stream_out_buffer_.get());

    return stream_out_;
}

template <typename T>
void compound_document::write_sector(binary_reader<T> &reader, sector_id id)
{
    out_->seekp(static_cast<std::streampos>(sector_data_start() + sector_size() * id));
    out_->write(reinterpret_cast<const char *>(reader.data() + reader.offset()),
        static_cast<std::streamsize>(std::min(sector_size(), static_cast<std::uint64_t>(reader.bytes() - reader.offset()))));
}

template <typename T>
void compound_document::write_short_sector(binary_reader<T> &reader, sector_id id)
{
    sector_chain chain = follow_chain(entries_.at(0).start, sat_);
    sector_id sector_id = chain.at(static_cast<std::size_t>(id / (sector_size() / short_sector_size())));
    std::uint64_t sector_offset = id % (sector_size() / short_sector_size()) * short_sector_size();
    out_->seekp(static_cast<std::streampos>(sector_data_start() + sector_size() * sector_id + sector_offset));
    out_->write(reinterpret_cast<const char *>(reader.data() + reader.offset()),
        static_cast<std::streamsize>(std::min(short_sector_size(), static_cast<std::uint64_t>(reader.bytes() - reader.offset()))));
}

template <typename T>
void compound_document::read_sector(sector_id id, binary_writer<T> &writer)
{
    in_->seekg(static_cast<std::streampos>(sector_data_start() + sector_size() * id));
    std::vector<byte> sector(sector_size(), 0);
    in_->read(reinterpret_cast<char *>(sector.data()), static_cast<std::streamsize>(sector_size()));
    writer.append(sector);
}

template <typename T>
void compound_document::read_sector_chain(sector_id start, binary_writer<T> &writer)
{
    for (sector_id link : follow_chain(start, sat_))
    {
        read_sector(link, writer);
    }
}

template <typename T>
void compound_document::read_sector_chain(sector_id start, binary_writer<T> &writer, sector_id offset, std::size_t count)
{
    sector_chain chain = follow_chain(start, sat_);

    for (std::size_t i = 0; i < count; ++i)
    {
        read_sector(chain.at(offset + i), writer);
    }
}

template <typename T>
void compound_document::read_short_sector(sector_id id, binary_writer<T> &writer)
{
    const sector_chain container_chain = follow_chain(entries_.at(0).start, sat_);
    std::vector<byte> container;
    binary_writer<byte> container_writer(container);

    for (sector_id sector : container_chain)
    {
        read_sector(sector, container_writer);
    }

    binary_reader<byte> container_reader(container);
    container_reader.offset(static_cast<std::size_t>(id * short_sector_size()));

    writer.append(container_reader, short_sector_size());
}

template <typename T>
void compound_document::read_short_sector_chain(sector_id start, binary_writer<T> &writer)
{
    for (sector_id link : follow_chain(start, ssat_))
    {
        read_short_sector(link, writer);
    }
}

template <typename T>
void compound_document::read_short_sector_chain(sector_id start, binary_writer<T> &writer, sector_id offset, std::size_t count)
{
    sector_chain chain = follow_chain(start, ssat_);

    for (std::size_t i = 0; i < count; ++i)
    {
        read_short_sector(chain.at(offset + i), writer);
    }
}

sector_id compound_document::allocate_sector()
{
    const auto sectors_per_sector = static_cast<std::size_t>(sector_size() / sizeof(sector_id));
    auto next_free_iter = std::find(sat_.begin(), sat_.end(), FREESECT);

    if (next_free_iter == sat_.end())
    {
        std::uint32_t next_msat_index = header_.num_msat_sectors;
        auto new_sat_sector_id = static_cast<sector_id>(sat_.size());

        msat_.push_back(new_sat_sector_id);
        write_msat();

        header_.msat.at(msat_.size() - 1) = new_sat_sector_id;
        ++header_.num_msat_sectors;
        write_header();

        sat_.resize(sat_.size() + sectors_per_sector, FREESECT);
        sat_.at(new_sat_sector_id) = FATSECT;

        binary_reader<sector_id> sat_reader(sat_);
        sat_reader.offset(next_msat_index * sectors_per_sector);
        write_sector(sat_reader, new_sat_sector_id);

        next_free_iter = std::find(sat_.begin(), sat_.end(), FREESECT);
    }

    auto next_free = static_cast<sector_id>(next_free_iter - sat_.begin());
    sat_.at(next_free) = ENDOFCHAIN;

    write_sat();

    std::vector<byte> empty_sector(sector_size());
    binary_reader<byte> empty_sector_reader(empty_sector);
    write_sector(empty_sector_reader, next_free);

    return next_free;
}

sector_chain compound_document::allocate_sectors(std::size_t count)
{
    if (count == 0) return {};

    sector_chain chain;
    chain.reserve(count);
    sector_id current = allocate_sector();

    for (std::size_t i = 1; i < count; ++i)
    {
        chain.push_back(current);
        sector_id next = allocate_sector();
        sat_.at(current) = next;
        current = next;
    }

    chain.push_back(current);
    write_sat();

    return chain;
}

sector_chain compound_document::follow_chain(sector_id start, const sector_chain &table)
{
    sector_chain chain;
    sector_id current = start;

    while (!is_invalid_sector(current))
    {
        chain.push_back(current);
        current = table.at(current);
    }

    return chain;
}

sector_chain compound_document::allocate_short_sectors(std::size_t count)
{
    if (count == 0) return {};

    sector_chain chain;
    chain.reserve(count);
    sector_id current = allocate_short_sector();

    for (std::size_t i = 1; i < count; ++i)
    {
        chain.push_back(current);
        sector_id next = allocate_short_sector();
        ssat_.at(current) = next;
        current = next;
    }

    chain.push_back(current);
    write_ssat();

    return chain;
}

sector_id compound_document::allocate_short_sector()
{
    const auto sectors_per_sector = static_cast<std::size_t>(sector_size() / sizeof(sector_id));
    auto next_free_iter = std::find(ssat_.begin(), ssat_.end(), FREESECT);

    if (next_free_iter == ssat_.end())
    {
        sector_id new_ssat_sector_id = allocate_sector();

        if (is_invalid_sector(header_.ssat_start))
        {
            header_.ssat_start = new_ssat_sector_id;
        }
        else
        {
            sector_chain ssat_chain = follow_chain(header_.ssat_start, sat_);
            sat_.at(last_elem(ssat_chain)) = new_ssat_sector_id;
            write_sat();
        }

        write_header();

        std::size_t old_size = ssat_.size();
        ssat_.resize(old_size + sectors_per_sector, FREESECT);

        binary_reader<sector_id> ssat_reader(ssat_);
        ssat_reader.offset(old_size / sectors_per_sector);
        write_sector(ssat_reader, new_ssat_sector_id);

        next_free_iter = std::find(ssat_.begin(), ssat_.end(), FREESECT);
    }

    ++header_.num_short_sectors;
    write_header();

    auto next_free = static_cast<sector_id>(next_free_iter - ssat_.begin());
    ssat_.at(next_free) = ENDOFCHAIN;

    write_ssat();

    const std::uint64_t short_sectors_per_sector = sector_size() / short_sector_size();
    const std::uint64_t required_container_sectors = next_free / short_sectors_per_sector + 1;

    if (required_container_sectors > 0)
    {
        if (is_invalid_sector(entries_.at(0).start))
        {
            entries_.at(0).start = allocate_sector();
            write_entry(0);
        }

        sector_chain container_chain = follow_chain(entries_.at(0).start, sat_);

        if (required_container_sectors > container_chain.size())
        {
            sat_.at(last_elem(container_chain)) = allocate_sector();
            write_sat();
        }
    }

    return next_free;
}

directory_id compound_document::next_empty_entry()
{
    directory_id entry_id = 0;

    for (; entry_id < entries_.size(); ++entry_id)
    {
        if (entries_.at(entry_id).type == compound_document_entry::entry_type::Empty)
        {
            return entry_id;
        }
    }

    // entry_id is now equal to entries_.size()

    if (is_invalid_sector(header_.directory_start))
    {
        header_.directory_start = allocate_sector();
    }
    else
    {
        sector_chain directory_chain = follow_chain(header_.directory_start, sat_);
        sat_.at(last_elem(directory_chain)) = allocate_sector();
        write_sat();
    }

    const auto entries_per_sector = static_cast<std::size_t>(sector_size() / COMPOUND_DOCUMENT_ENTRY_SIZE);

    entries_.reserve(entries_.size() + entries_per_sector);
    for (std::size_t i = 0; i < entries_per_sector; ++i)
    {
        entries_.emplace_back();
        write_entry(entry_id + static_cast<directory_id>(i));
    }

    return entry_id;
}

directory_id compound_document::insert_entry(
    const std::string &name,
    compound_document_entry::entry_type type)
{
    directory_id entry_id = next_empty_entry();
    compound_document_entry &entry = entries_.at(entry_id);

    directory_id parent_id = 0;
    std::vector<std::string> split = split_path(name);
    std::string filename = last_elem(split);
    split.pop_back();

    if (split.size() > 1)
    {
        std::string joined_path = join_path(split);
        parent_id = find_entry(joined_path, compound_document_entry::entry_type::UserStorage);

        if (is_invalid_entry(parent_id))
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

std::uint64_t compound_document::sector_data_start()
{
    return sizeof(compound_document_header);
}

bool compound_document::contains_entry(const std::string &path,
    compound_document_entry::entry_type type)
{
    return !is_invalid_entry(find_entry(path, type));
}

directory_id compound_document::find_entry(const std::string &name,
    compound_document_entry::entry_type type)
{
    if (type == compound_document_entry::entry_type::RootStorage
        && (name == "/" || name == "/Root Entry")) return 0;

    directory_id entry_id = 0;

    for (const compound_document_entry &entry : entries_)
    {
        if (entry.type == type && tree_path(entry_id) == name)
        {
            return entry_id;
        }

        ++entry_id;
    }

    return NOSTREAM;
}

void compound_document::print_directory()
{
    directory_id entry_id = 0;

    for (const compound_document_entry &entry : entries_)
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
    for (std::size_t entry_id = 0; entry_id < entries_.size(); ++entry_id)
    {
        write_entry(static_cast<directory_id>(entry_id));
    }
}

void compound_document::read_directory()
{
    const std::uint64_t entries_per_sector = sector_size() / COMPOUND_DOCUMENT_ENTRY_SIZE;
    const std::size_t num_entries = static_cast<std::size_t>(
        follow_chain(header_.directory_start, sat_).size() * entries_per_sector);

    entries_.reserve(entries_.size() + num_entries);
    for (std::size_t entry_id = 0; entry_id < num_entries; ++entry_id)
    {
        entries_.emplace_back();
        read_entry(static_cast<directory_id>(entry_id));
    }

    std::vector<directory_id> stack;
    std::vector<directory_id> storage_siblings;
    std::vector<directory_id> stream_siblings;

    std::vector<directory_id> directory_stack;
    directory_stack.push_back(0u);

    while (!directory_stack.empty())
    {
        directory_id current_storage_id = directory_stack.back();
        directory_stack.pop_back();

        if (is_invalid_entry(tree_child(current_storage_id))) continue;

        std::vector<directory_id> storage_stack;
        directory_id storage_root_id = tree_child(current_storage_id);
        parent_[storage_root_id] = NOSTREAM;
        storage_stack.push_back(storage_root_id);

        while (!storage_stack.empty())
        {
            directory_id current_entry_id = storage_stack.back();
            const compound_document_entry &current_entry = entries_.at(current_entry_id);
            storage_stack.pop_back();

            parent_storage_[current_entry_id] = current_storage_id;

            if (current_entry.type == compound_document_entry::entry_type::UserStorage)
            {
                directory_stack.push_back(current_entry_id);
            }

            if (!is_invalid_entry(tree_left(current_entry_id)))
            {
                storage_stack.push_back(tree_left(current_entry_id));
                tree_parent(tree_left(current_entry_id)) = current_entry_id;
            }

            if (!is_invalid_entry(tree_right(current_entry_id)))
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

    tree_left(new_id) = NOSTREAM;
    tree_right(new_id) = NOSTREAM;

    if (is_invalid_entry(tree_root(new_id)))
    {
        if (new_id != 0)
        {
            tree_root(new_id) = new_id;
        }

        tree_color(new_id) = entry_color::Black;
        tree_parent(new_id) = NOSTREAM;

        return;
    }

    // normal tree insert
    // (will probably unbalance the tree, fix after)
    directory_id x = tree_root(new_id);
    directory_id y = NOSTREAM;

    while (!is_invalid_entry(x))
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
    directory_id storage_id = parent_storage_.at(id);
    std::vector<std::string> result;

    while (storage_id > 0)
    {
        storage_id = parent_storage_.at(storage_id);
        result.emplace_back(entries_.at(storage_id).name());
    }

    return "/" + join_path(result) + entries_.at(id).name();
}

void compound_document::tree_rotate_left(directory_id x)
{
    directory_id y = tree_right(x);

    // turn y's left subtree into x's right subtree
    tree_right(x) = tree_left(y);

    if (!is_invalid_entry(tree_left(y)))
    {
        tree_parent(tree_left(y)) = x;
    }

    // link x's parent to y
    tree_parent(y) = tree_parent(x);

    if (is_invalid_entry(tree_parent(x)))
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
    directory_id x = tree_left(y);

    // turn x's right subtree into y's left subtree
    tree_left(y) = tree_right(x);

    if (!is_invalid_entry(tree_right(x)))
    {
        tree_parent(tree_right(x)) = y;
    }

    // link y's parent to x
    tree_parent(x) = tree_parent(y);

    if (is_invalid_entry(tree_parent(y)))
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
            directory_id y = tree_right(tree_parent(tree_parent(x)));

            if (!is_invalid_entry(y) && tree_color(y) == entry_color::Red)
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
            directory_id y = tree_left(tree_parent(tree_parent(x)));

            if (!is_invalid_entry(y) && tree_color(y) == entry_color::Red)
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
    return entries_.at(id).prev;
}

directory_id &compound_document::tree_right(directory_id id)
{
    return entries_.at(id).next;
}

directory_id &compound_document::tree_parent(directory_id id)
{
    // Note: the parent will be created, if it does not yet exist. This is fine.
    return parent_[id];
}

directory_id &compound_document::tree_root(directory_id id)
{
    return tree_child(parent_storage_.at(id));
}

directory_id &compound_document::tree_child(directory_id id)
{
    return entries_.at(id).child;
}

std::string compound_document::tree_key(directory_id id)
{
    return entries_.at(id).name();
}

compound_document_entry::entry_color &compound_document::tree_color(directory_id id)
{
    return entries_.at(id).color;
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
    if (std::any_of(header_.header_clsid.begin(), header_.header_clsid.end(), [](std::uint8_t i) { return i != 0; }))
    {
        std::string exception_str = "invalid header CLSID, expected only zeros but got: ";
        for (std::uint8_t val : header_.header_clsid)
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
    if (std::any_of(header_.reserved.begin(), header_.reserved.end(), [](std::uint8_t i) { return i != 0; }))
    {
        std::string exception_str = "invalid reserved field, expected only zeros but got: ";
        for (std::uint8_t val : header_.reserved)
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

        if (std::any_of(remaining.begin(), remaining.end(), [](std::uint8_t i) { return i != 0; }))
        {
            std::string exception_str = "invalid remaining bytes in header (major version 4), expected only zeros but got: ";
            for (std::uint8_t val : remaining)
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

    sector_id msat_sector = header_.extra_msat_start;
    binary_writer<sector_id> msat_writer(msat_);

    for (std::uint32_t i = 0u; i < header_.num_msat_sectors; ++i)
    {
        if (i < 109u)
        {
            msat_writer.write(header_.msat.at(i));
        }
        else
        {
            read_sector(msat_sector, msat_writer);

            msat_sector = last_elem(msat_);
            msat_.pop_back();
        }
    }
}

void compound_document::read_sat()
{
    sat_.clear();
    binary_writer<sector_id> sat_writer(sat_);

    for (sector_id msat_sector : msat_)
    {
        read_sector(msat_sector, sat_writer);
    }
}

void compound_document::read_ssat()
{
    ssat_.clear();
    binary_writer<sector_id> ssat_writer(ssat_);

    for (sector_id ssat_sector : follow_chain(header_.ssat_start, sat_))
    {
        read_sector(ssat_sector, ssat_writer);
    }
}

std::string compound_document_entry::format_info(
    directory_id entry_id,
    sector_id sector_id,
    /// IMPORTANT: only show the name after the name and its length have been validated!
    bool show_entry_name) const
{
    // The formatted IDs should be as short as possible to keep the exception message readable - so we do not add leading zeros.
    std::string message = "(entry " + fmt::format("0x{:X}", entry_id);
    if (show_entry_name)
    {
        message += " with name \"";
        // Only add the name if the conversion does not throw an exception itself!
        try
        {
            message += name();
        }
        catch (const std::exception &ex)
        {
            message += "INVALID (";
            message += ex.what();
            message.push_back(')');
        }
        message.push_back('"');
    }
    message += " of type " + std::to_string(static_cast<int>(type)) +
        " in sector " + fmt::format("0x{:X}", sector_id) + ")";
    return message;
}

void check_empty_entry(
    const compound_document_entry &entry,
    directory_id id,
    sector_id directory_sector)
{
    if (entry.type != compound_document_entry::entry_type::Empty)
    {
        throw xlnt::invalid_parameter("invalid entry type " +
            entry.format_info(id, directory_sector, false) +
            ", expected Empty but got " + std::to_string(static_cast<int>(entry.type)));
    }

    // Free (unused) directory entries are marked with Object Type 0x0 (unknown or unallocated). The
    // entire directory entry must consist of all zeroes except for the child, right sibling, and left sibling
    // pointers, which must be initialized to NOSTREAM (0xFFFFFFFF).

    // NOTE: Some implementations seem to not initialize this buffer at all, so we cannot check it for correctness.
    /*if (std::any_of(entry.name_array.begin(), entry.name_array.end(), [](char16_t i) { return i != 0; }))
    {
        std::string exception_str = "invalid entry name " +
            entry.format_info(id, directory_sector, false) +
            ", expected all zeros but got: ";
        for (char16_t val : entry.name_array)
        {
            exception_str += fmt::format("{:04x} ", static_cast<std::uint16_t>(val));
        }
        throw xlnt::invalid_file(exception_str);
    }*/

    if (entry.name_length != 0)
    {
        throw xlnt::invalid_file("invalid entry name length " + entry.format_info(id, directory_sector, false) +
            ", expected 0 but got " + std::to_string(entry.name_length));
    }

    if (static_cast<std::uint8_t>(entry.color) != 0)
    {
        throw xlnt::invalid_file("invalid entry color " + entry.format_info(id, directory_sector, false) +
            ", expected 0 but got " + std::to_string(static_cast<int>(entry.color)));
    }

    if (entry.prev != NOSTREAM || entry.next != NOSTREAM || entry.child != NOSTREAM)
    {
        throw xlnt::invalid_file("empty entry contains invalid child or sibling " +
            entry.format_info(id, directory_sector, false) +
            "; prev = " + fmt::format("0x{:08X}", (entry.prev)) +
            "; next = " + fmt::format("0x{:08X}", (entry.next)) +
            "; child = " + fmt::format("0x{:08X}", (entry.child)));
    }

    if (std::any_of(entry.clsid.begin(), entry.clsid.end(), [](std::uint8_t i) { return i != 0; }))
    {
        std::string exception_str = "invalid entry CLSID " + entry.format_info(id, directory_sector, false) +
            ", expected all zeros but got: ";
        for (std::uint8_t val : entry.clsid)
        {
            exception_str += fmt::format("{:02x} ", val);
        }
        throw xlnt::invalid_file(exception_str);
    }

    if (entry.state_bits != 0)
    {
        throw xlnt::invalid_file("invalid entry state bits " + entry.format_info(id, directory_sector, false) +
            ", expected 0 but got " + std::to_string(entry.state_bits));
    }

    // NOTE: some implementations seem to use the timestamp 116444736000000000, which is 1970-01-01 00:00:00 UTC.
    if (entry.creation_time != 0 && entry.creation_time != 116444736000000000)
    {
        throw xlnt::invalid_file("invalid entry creation time " + entry.format_info(id, directory_sector, false) +
            ", expected 0 or 116444736000000000, but got " + std::to_string(entry.creation_time));
    }

    // NOTE: some implementations seem to use the timestamp 116444736000000000, which is 1970-01-01 00:00:00 UTC.
    if (entry.modified_time != 0 && entry.modified_time != 116444736000000000)
    {
        throw xlnt::invalid_file("invalid entry modification time " + entry.format_info(id, directory_sector, false) +
            ", expected 0 or 116444736000000000, but got " + std::to_string(entry.modified_time));
    }

    // According to the specification (see above), it must be 0, but it seems that some immplementations
    // initialize it with ENDOFCHAIN or FREESECT, which is honestly not wrong either. So let's accept that.
    if (entry.start != 0 && entry.start != ENDOFCHAIN && entry.start != FREESECT)
    {
        throw xlnt::invalid_file("invalid entry start sector location " + entry.format_info(id, directory_sector, false) +
            ", expected 0 or ENDOFCHAIN (0xFFFFFFFE) or FREESECT (0xFFFFFFFF), but got " + format_hex(entry.start));
    }

    if (entry.size != 0)
    {
        throw xlnt::invalid_file("invalid entry stream size " + entry.format_info(id, directory_sector, false) +
            ", expected 0 but got " + std::to_string(entry.size));
    }
}

void check_non_empty_entry(
    const compound_document_entry &entry,
    directory_id id,
    sector_id directory_sector)
{
    if (entry.type == compound_document_entry::entry_type::Empty)
    {
        throw xlnt::invalid_parameter("invalid entry type " +
            entry.format_info(id, directory_sector, false) +
            ", expected different than Empty but got Empty");
    }

    // First check the length, as we'll need this for the string itself.
    // Directory Entry Name Length (2 bytes): This field MUST match the length of the Directory Entry
    // Name Unicode string in bytes. The length MUST be a multiple of 2 and include the terminating null
    // character in the count. This length MUST NOT exceed 64, the maximum size of the Directory Entry
    // Name field.
    if (entry.name_length < 2 || entry.name_length > 64)
    {
        throw xlnt::invalid_file("invalid entry name length " +
            entry.format_info(id, directory_sector, false) +
            ", expected >= 2 and <= 64, but got " + std::to_string(entry.name_length));
    }
    else if (entry.name_length % 2 != 0)
    {
        throw xlnt::invalid_file("invalid entry name length " +
            entry.format_info(id, directory_sector, false) +
            ", which must be a multiple of 2, but got " + std::to_string(entry.name_length));
    }

    // Directory Entry Name (64 bytes): This field MUST contain a Unicode string for the storage or
    // stream name encoded in UTF-16. The name MUST be terminated with a UTF-16 terminating null
    // character. Thus, storage and stream names are limited to 32 UTF-16 code points, including the
    // terminating null character. When locating an object in the compound file except for the root
    // storage, the directory entry name is compared by using a special case-insensitive uppercase
    // mapping, described in Red-Black Tree. The following characters are illegal and MUST NOT be part
    // of the name: '/', '\', ':', '!'.
    std::uint16_t name_length_characters = (entry.name_length / 2) - 1; // does NOT include \0 at the end
    if (entry.name_array.at(name_length_characters) != u'\0')
    {
        std::string exception_str = "invalid entry name " +
            entry.format_info(id, directory_sector, false) +
            ", which must be terminated with \\0 but is terminated with " +
            fmt::format("0x{:04X}", static_cast<std::uint16_t>(entry.name_array.at(name_length_characters))) +
            "\nString has a length of " + std::to_string(name_length_characters) + " characters (" +
            std::to_string(entry.name_length) + " bytes including \\0). Full buffer contents:\n";
        for (char16_t val : entry.name_array)
        {
            exception_str += fmt::format("{:04x} ", static_cast<std::uint16_t>(val));
        }

        throw xlnt::invalid_file(exception_str);
    }

    for (std::uint16_t n = 0; n < name_length_characters; ++n)
    {
        char16_t curr = entry.name_array.at(n);
        if (curr == u'/' || curr == u'\\' || curr == u':' || curr == u'!')
        {
            throw xlnt::invalid_file("invalid entry name " + entry.format_info(id, directory_sector, true) +
                ", which contains invalid character " +
                fmt::format("0x{:04X}", static_cast<std::uint16_t>(curr)) + " at position " + std::to_string(n));
        }
    }

    // Object Type (1 byte): This field MUST be 0x00, 0x01, 0x02, or 0x05, depending on the actual type
    // of object. All other values are not valid.
    // --------------------------------
    // NOTE: the empty type is handled in check_empty_entry().
    if (static_cast<std::uint8_t>(entry.type) != 0x01 && // UserStorage
        static_cast<std::uint8_t>(entry.type) != 0x02 && // UserStream
        static_cast<std::uint8_t>(entry.type) != 0x05) // RootStorage
    {
        throw xlnt::invalid_file("invalid entry object type " + entry.format_info(id, directory_sector, true) +
            ", expected 0, 1, 2 or 5 but got " + std::to_string(static_cast<int>(entry.type)));
    }

    // Color Flag (1 byte): This field MUST be 0x00 (red) or 0x01 (black). All other values are not valid.
    if (static_cast<std::uint8_t>(entry.color) != 0 && static_cast<std::uint8_t>(entry.color) != 1)
    {
        throw xlnt::invalid_file("invalid entry color " + entry.format_info(id, directory_sector, true) +
            ", expected 0 or 1, but got " + std::to_string(static_cast<int>(entry.color)));
    }

    // CLSID (16 bytes): This field contains an object class GUID, if this entry is for a storage object or
    // root storage object. For a stream object, this field MUST be set to all zeroes. A value containing all
    // zeroes in a storage or root storage directory entry is valid, and indicates that no object class is
    // associated with the storage. If an implementation of the file format enables applications to create
    // storage objects without explicitly setting an object class GUID, it MUST write all zeroes by default.
    // If this value is not all zeroes, the object class GUID can be used as a parameter to start
    // applications.
    if (entry.type == compound_document_entry::entry_type::UserStream &&
        std::any_of(entry.clsid.begin(), entry.clsid.end(), [](std::uint8_t i) { return i != 0; }))
    {
        std::string exception_str = "invalid entry CLSID " + entry.format_info(id, directory_sector, true) +
            " for UserStream type, expected all zeros but got: ";
        for (std::uint8_t val : entry.clsid)
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
    // --------------------------------
    // NOTE: unfortunately cannot be enforced, as some files:
    // - have a root entry with timestamp 116444736000000000, which is 1970-01-01 00:00:00 UTC
    // - have a stream with an actual timestamp
    /*if ((entry.type == compound_document_entry::entry_type::UserStream ||
        entry.type == compound_document_entry::entry_type::RootStorage) &&
        entry.creation_time != 0)
    {
        throw xlnt::invalid_file("invalid entry creation time " + entry.format_info(id, directory_sector, true) +
            " for type " + std::to_string(static_cast<int>(entry.type)) +
            ", expected 0 but got " + std::to_string(entry.creation_time));
    }*/

    // Modified Time (8 bytes): This field contains the modification time for a storage object, or all
    // zeroes to indicate that the modified time of the storage object was not recorded. The Windows
    // FILETIME structure is used to represent this field in UTC. For a stream object, this field MUST be
    // all zeroes. For a root storage object, this field MAY<2> be set to all zeroes, and the modified time
    // is retrieved or set on the compound file itself.
    // --------------------------------
    // NOTE: unfortunately cannot be enforced, as some files have a stream with an actual timestamp.
    /*if (entry.type == compound_document_entry::entry_type::UserStream &&
        entry.modified_time != 0)
    {
        throw xlnt::invalid_file("invalid entry modification time " + entry.format_info(id, directory_sector, true) +
            " for type UserStream, expected 0 but got " + std::to_string(entry.modified_time));
    }*/

    // Starting Sector Location (4 bytes): This field contains the first sector location if this is a stream
    // object. For a root storage object, this field MUST contain the first sector of the mini stream, if the
    // mini stream exists. For a storage object, this field MUST be set to all zeroes.
    // --------------------------------
    // It seems that some immplementations initialize it with FREESECT,
    // which is honestly not wrong either. So let's accept that.
    if (entry.type == compound_document_entry::entry_type::UserStorage &&
        !(entry.start == 0 || entry.start == FREESECT))
    {
        throw xlnt::invalid_file("invalid entry start sector location " + entry.format_info(id, directory_sector, true) +
            " for type UserStorage, expected 0 or FREESECT (0xFFFFFFFF), but got " + format_hex(entry.start));
    }

    // Stream Size (8 bytes): This 64-bit integer field contains the size of the user-defined data if this is
    // a stream object. For a root storage object, this field contains the size of the mini stream. For a
    // storage object, this field MUST be set to all zeroes.
    if (entry.type == compound_document_entry::entry_type::UserStorage &&
        entry.size != 0)
    {
        throw xlnt::invalid_file("invalid entry stream size " + entry.format_info(id, directory_sector, true) +
            " for type UserStorage, expected 0 but got " + std::to_string(entry.size));
    }
}

void compound_document::read_entry(directory_id id)
{
    const sector_chain directory_chain = follow_chain(header_.directory_start, sat_);
    const std::uint64_t entries_per_sector = sector_size() / COMPOUND_DOCUMENT_ENTRY_SIZE;
    const sector_id directory_sector = directory_chain.at(static_cast<std::size_t>(id / entries_per_sector));
    const std::uint64_t offset = sector_size() * directory_sector + ((id % entries_per_sector) * COMPOUND_DOCUMENT_ENTRY_SIZE);

    in_->seekg(static_cast<std::streamoff>(sector_data_start() + offset), std::ios::beg);
    compound_document_entry &entry = entries_.at(id);
    // Read the fields manually due to struct padding (larger sizeof than 128 bytes).
    in_->read(reinterpret_cast<char *>(entry.name_array.data()), sizeof(entry.name_array));
    in_->read(reinterpret_cast<char *>(&entry.name_length), sizeof(entry.name_length));
    in_->read(reinterpret_cast<char *>(&entry.type), sizeof(entry.type));
    in_->read(reinterpret_cast<char *>(&entry.color), sizeof(entry.color));
    in_->read(reinterpret_cast<char *>(&entry.prev), sizeof(entry.prev));
    in_->read(reinterpret_cast<char *>(&entry.next), sizeof(entry.next));
    in_->read(reinterpret_cast<char *>(&entry.child), sizeof(entry.child));
    in_->read(reinterpret_cast<char *>(entry.clsid.data()), sizeof(entry.clsid));
    in_->read(reinterpret_cast<char *>(&entry.state_bits), sizeof(entry.state_bits));
    in_->read(reinterpret_cast<char *>(&entry.creation_time), sizeof(entry.creation_time));
    in_->read(reinterpret_cast<char *>(&entry.modified_time), sizeof(entry.modified_time));
    in_->read(reinterpret_cast<char *>(&entry.start), sizeof(entry.start));
    in_->read(reinterpret_cast<char *>(&entry.size), sizeof(entry.size));

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
    if (header_.major_version == 3 && entry.size > 0x80000000)
    {
        // Note: the only allowed byte order is little-endian.
        entry.size = entry.size & 0x0000FFFF;
    }

    if (entry.type == compound_document_entry::entry_type::Empty)
    {
        check_empty_entry(entry, id, directory_sector);
    }
    else
    {
        check_non_empty_entry(entry, id, directory_sector);
    }
}

void compound_document::write_header()
{
    out_->seekp(0, std::ios::beg);
    out_->write(reinterpret_cast<char *>(&header_), sizeof(compound_document_header));
}

void compound_document::write_msat()
{
    sector_id msat_sector = header_.extra_msat_start;

    for (std::uint32_t i = 0u; i < header_.num_msat_sectors; ++i)
    {
        if (i < 109u)
        {
            header_.msat.at(i) = msat_.at(i);
        }
        else
        {
            std::vector<sector_id> sector;
            binary_writer<sector_id> sector_writer(sector);

            read_sector(msat_sector, sector_writer);

            msat_sector = last_elem(sector);
            sector.pop_back();

            std::copy(sector.begin(), sector.end(), std::back_inserter(msat_));
        }
    }
}

void compound_document::write_sat()
{
    binary_reader<sector_id> sector_reader(sat_);

    for (sector_id sat_sector : msat_)
    {
        write_sector(sector_reader, sat_sector);
    }
}

void compound_document::write_ssat()
{
    binary_reader<sector_id> sector_reader(ssat_);

    for (sector_id ssat_sector : follow_chain(header_.ssat_start, sat_))
    {
        write_sector(sector_reader, ssat_sector);
    }
}

void compound_document::write_entry(directory_id id)
{
    const sector_chain directory_chain = follow_chain(header_.directory_start, sat_);
    const std::uint64_t entries_per_sector = sector_size() / COMPOUND_DOCUMENT_ENTRY_SIZE;
    const sector_id directory_sector = directory_chain.at(static_cast<std::size_t>(id / entries_per_sector));
    const std::uint64_t offset = sector_data_start() + sector_size() * directory_sector
        + ((id % entries_per_sector) * COMPOUND_DOCUMENT_ENTRY_SIZE);

    out_->seekp(static_cast<std::streamoff>(offset), std::ios::beg);
    const compound_document_entry &entry = entries_.at(id);
    // Write the fields manually due to struct padding (larger sizeof than 128 bytes).
    out_->write(reinterpret_cast<const char *>(entry.name_array.data()), sizeof(entry.name_array));
    out_->write(reinterpret_cast<const char *>(&entry.name_length), sizeof(entry.name_length));
    out_->write(reinterpret_cast<const char *>(&entry.type), sizeof(entry.type));
    out_->write(reinterpret_cast<const char *>(&entry.color), sizeof(entry.color));
    out_->write(reinterpret_cast<const char *>(&entry.prev), sizeof(entry.prev));
    out_->write(reinterpret_cast<const char *>(&entry.next), sizeof(entry.next));
    out_->write(reinterpret_cast<const char *>(&entry.child), sizeof(entry.child));
    out_->write(reinterpret_cast<const char *>(entry.clsid.data()), sizeof(entry.clsid));
    out_->write(reinterpret_cast<const char *>(&entry.state_bits), sizeof(entry.state_bits));
    out_->write(reinterpret_cast<const char *>(&entry.creation_time), sizeof(entry.creation_time));
    out_->write(reinterpret_cast<const char *>(&entry.modified_time), sizeof(entry.modified_time));
    out_->write(reinterpret_cast<const char *>(&entry.start), sizeof(entry.start));
    out_->write(reinterpret_cast<const char *>(&entry.size), sizeof(entry.size));
}

} // namespace detail
} // namespace xlnt
