// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2024-2026 xlnt-community
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file

#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

#include <xlnt/utils/exceptions.hpp>

namespace xlnt {
namespace detail {

using byte = std::uint8_t;

template<typename T>
class binary_reader
{
public:
    binary_reader() = delete;

    binary_reader(const std::vector<T> &vector)
        : vector_(&vector),
          data_(nullptr),
          size_(0)
    {
    }

    binary_reader(const T *source_data, std::size_t size)
        : vector_(nullptr),
          data_(source_data),
          size_(size)
    {
    }

    const T *data() const
    {
        return vector_ == nullptr ? data_ : vector_->data();
    }

    void offset(std::size_t offset)
    {
        offset_ = offset;
    }

    std::size_t offset() const
    {
        return offset_;
    }

    void reset()
    {
        offset_ = 0;
    }

    template<typename U>
    U read()
    {
        return read_reference<U>();
    }

    template<typename U>
    const U *read_pointer()
    {
        static_assert(sizeof(U) % sizeof(T) == 0, "sizeof(U) MUST be a multiple of sizeof(T)");

        const std::size_t num_elements_to_read = sizeof(U) / sizeof(T);
        if (offset_ + num_elements_to_read > count())
        {
            throw xlnt::invalid_parameter("Reading past end while trying to read 1 element of size " + std::to_string(sizeof(U)) +
                ": attempted to read from offset at element " + std::to_string(offset_) +
                " (byte " + std::to_string(offset_ * sizeof(T)) + ") reading " + std::to_string(num_elements_to_read) +
                " elements (" + std::to_string(num_elements_to_read * sizeof(T)) + " bytes), although the reader only has " +
                std::to_string(count()) + " elements (" + std::to_string(bytes()) + " bytes)");
        }

        const auto result = reinterpret_cast<const U *>(data() + offset_);
        offset_ += num_elements_to_read;

        return result;
    }

    template<typename U>
    const U &read_reference()
    {
        return *read_pointer<U>();
    }

    template<typename U>
    std::vector<U> as_vector() const
    {
        // Relax requirements (no static_assert) to allow reading as a vector of bytes.
        if (bytes() % sizeof(U) != 0)
        {
            throw xlnt::invalid_parameter("Cannot copy " + std::to_string(count()) + " elements with an element size of " +
                std::to_string(sizeof(T)) + " bytes (total of " + std::to_string(bytes()) + " bytes) to vector with an element size of " +
                std::to_string(sizeof(U)) + " bytes");
        }

        if (count() == 0)
        {
            return {}; // otherwise std::vector::data() could be nullptr which causes undefined behavior with std::memcpy
        }

        auto result = std::vector<U>(bytes() / sizeof(U), U());
        std::memcpy(result.data(), data(), bytes());

        return result;
    }

    template<typename U>
    std::vector<U> read_vector(std::size_t count)
    {
        // Relax requirements (no static_assert) to allow reading as a vector of bytes.
        if ((count * sizeof(U)) % sizeof(T) != 0)
        {
            throw xlnt::invalid_parameter("Cannot copy " + std::to_string(count) + " elements with an element size of " +
                std::to_string(sizeof(U)) + " bytes (total of " + std::to_string(count * sizeof(U)) + " bytes) from vector with an element size of " +
                std::to_string(sizeof(T)) + " bytes");
        }

        const std::size_t num_elements_to_read = count * sizeof(U) / sizeof(T);
        if (offset_ + num_elements_to_read > this->count())
        {
            throw xlnt::invalid_parameter("Reading past end while trying to read " + std::to_string(count) +
                " elements of size " + std::to_string(sizeof(U)) + " (total of " + std::to_string(count * sizeof(U)) +
                " bytes): attempted to read from offset at element " + std::to_string(offset_) +
                " (byte " + std::to_string(offset_ * sizeof(T)) + ") reading " + std::to_string(num_elements_to_read) +
                " elements (" + std::to_string(num_elements_to_read * sizeof(T)) + " bytes), although the reader only has " +
                std::to_string(this->count()) + " elements (" + std::to_string(bytes()) + " bytes)");
        }

        if (count == 0 || this->count() == 0)
        {
            return {}; // otherwise std::vector::data() could be nullptr which causes undefined behavior with std::memcpy
        }

        auto result = std::vector<U>(count, U());
        std::memcpy(result.data(), data() + offset_, count * sizeof(U));
        offset_ += num_elements_to_read;

        return result;
    }

    std::size_t count() const
    {
        return vector_ != nullptr ? vector_->size() : size_;
    }

    std::size_t bytes() const
    {
        return count() * sizeof(T);
    }

private:
    std::size_t offset_ = 0;
    const std::vector<T> *vector_ = nullptr;
    const T *data_ = nullptr;
    const std::size_t size_ = 0;
};

template<typename T>
class binary_writer
{
public:
    binary_writer(std::vector<T> &bytes)
        : data_(&bytes)
    {
    }

    std::vector<T> &data()
    {
        return *data_;
    }

    // Make the bytes of the data pointed to by this writer equivalent to those in the given vector
    // sizeof(U) should be a multiple of sizeof(T)
    template<typename U>
    void assign(const std::vector<U> &data)
    {
        // Relax requirements (no static_assert) to allow writing a vector of bytes.
        if ((data.size() * sizeof(U)) % sizeof(T) != 0)
        {
            throw xlnt::invalid_parameter("Error trying to assign from vector: invalid size requirements: "
                "trying to assign " + std::to_string(data.size()) + " elements with a size of " + std::to_string(sizeof(U)) +
                " bytes (" + std::to_string(data.size() * sizeof(U)) + " bytes in total) but writer has elements with a size of " +
                std::to_string(sizeof(T)) + " bytes.");
        }

        resize(data.size() * sizeof(U) / sizeof(T));

        // otherwise std::vector::data() could be nullptr which causes undefined behavior with std::memcpy
        if (data.size() != 0)
        {
            std::memcpy(data_->data(), data.data(), bytes());
        }
    }

    // Make the bytes of the data pointed to by this writer equivalent to those in the given string
    // sizeof(U) should be a multiple of sizeof(T)
    // append_NUL specifies whether the NUL terminator should be added to the binary representation.
    template<typename U>
    void assign(const std::basic_string<U> &string, bool append_NUL)
    {
        static_assert(sizeof(U) % sizeof(T) == 0, "sizeof(U) MUST be a multiple of sizeof(T)");

        const std::size_t string_length = string.length() + (append_NUL ? 1 : 0);
        resize(string_length * sizeof(U) / sizeof(T));
        std::memcpy(data_->data(), string.data(), bytes());
    }

    void offset(std::size_t new_offset)
    {
        offset_ = new_offset;
    }

    std::size_t offset() const
    {
        return offset_;
    }

    void reset()
    {
        offset_ = 0;
        data_->clear();
    }

    template<typename U>
    void write(U value)
    {
        static_assert(sizeof(U) % sizeof(T) == 0, "sizeof(U) MUST be a multiple of sizeof(T)");

        if (offset_ > count())
        {
            throw xlnt::invalid_attribute("Error trying to write single value: invalid writer offset at element " + std::to_string(offset_) +
                " (byte " + std::to_string(offset_ * sizeof(T)) + "), but writer only has " +
                std::to_string(count()) + " elements (" + std::to_string(bytes()) + " bytes)");
        }

        const auto num_bytes = sizeof(U);
        const auto remaining_bytes = bytes() - offset() * sizeof(T);

        if (remaining_bytes < num_bytes)
        {
            extend((num_bytes - remaining_bytes) / sizeof(T));
        }

        std::memcpy(data_->data() + offset(), &value, num_bytes);
        offset_ += num_bytes / sizeof(T);
    }

    std::size_t count() const
    {
        return data_->size();
    }

    std::size_t bytes() const
    {
        return count() * sizeof(T);
    }

    void resize(std::size_t new_size, byte fill = 0)
    {
        data_->resize(new_size, fill);
    }

    void extend(std::size_t amount, byte fill = 0)
    {
        data_->resize(count() + amount, fill);
    }

    std::vector<byte>::iterator iterator()
    {
        return data_->begin() + static_cast<std::ptrdiff_t>(offset());
    }

    template<typename U>
    void append(const std::vector<U> &data)
    {
        // Relax requirements (no static_assert) to allow writing a vector of bytes.
        if ((data.size() * sizeof(U)) % sizeof(T) != 0)
        {
            throw xlnt::invalid_parameter("Error trying to append from vector: invalid size requirements: "
                "trying to append " + std::to_string(data.size()) + " elements with a size of " + std::to_string(sizeof(U)) +
                " bytes (" + std::to_string(data.size() * sizeof(U)) + " bytes in total) but writer has elements with a size of " +
                std::to_string(sizeof(T)) + " bytes.");
        }

        if (offset_ > count())
        {
            throw xlnt::invalid_attribute("Error trying to append from vector: invalid writer offset at element " + std::to_string(offset_) +
                " (byte " + std::to_string(offset_ * sizeof(T)) + "), but writer only has " +
                std::to_string(count()) + " elements (" + std::to_string(bytes()) + " bytes)");
        }

        if (data.size() == 0)
        {
            return; // otherwise std::vector::data() could be nullptr which causes undefined behavior with std::memcpy
        }

        const auto num_bytes = sizeof(U) * data.size();
        const auto remaining_bytes = bytes() - offset() * sizeof(T);

        if (remaining_bytes < num_bytes)
        {
            extend((num_bytes - remaining_bytes) / sizeof(T));
        }

        std::memcpy(data_->data() + offset_, data.data(), num_bytes);
        offset_ += num_bytes / sizeof(T);
    }

    /// <summary>
    /// Reads reader_element_count elements from the reader.
    /// Assumes that so many elements can be read.
    /// If the reader does not contain at least reader_element_count elements,
    /// an invalid_parameter exception will be thrown.
    /// </summary>
    template<typename U>
    void append(binary_reader<U> &reader, std::size_t reader_element_count)
    {
        // Relax requirements (no static_assert) to allow writing a vector of bytes.
        if ((reader_element_count * sizeof(U)) % sizeof(T) != 0)
        {
            throw xlnt::invalid_parameter("Error trying to append from vector: invalid size requirements: "
                "trying to append " + std::to_string(reader_element_count) + " elements with a size of " + std::to_string(sizeof(U)) +
                " bytes (" + std::to_string(reader_element_count * sizeof(U)) + " bytes in total) but writer has elements with a size of " +
                std::to_string(sizeof(T)) + " bytes.");
        }

        if (offset_ > count())
        {
            throw xlnt::invalid_attribute("Error trying to append from reader: invalid writer offset at element " + std::to_string(offset_) +
                " (byte " + std::to_string(offset_ * sizeof(T)) + "), but writer only has " +
                std::to_string(count()) + " elements (" + std::to_string(bytes()) + " bytes)");
        }

        if (reader.offset() + reader_element_count > reader.count())
        {
            throw xlnt::invalid_parameter("Reading past end while trying to append from reader: "
                "attempted to read from offset at element " + std::to_string(reader.offset()) +
                " (byte " + std::to_string(reader.offset() * sizeof(U)) + ") reading " + std::to_string(reader_element_count) +
                " elements (" + std::to_string(reader_element_count * sizeof(U)) + " bytes), although the reader only has " +
                std::to_string(reader.count()) + " elements (" + std::to_string(reader.bytes()) + " bytes)");
        }

        if (reader.count() == 0)
        {
            return; // otherwise std::vector::data() could be nullptr which causes undefined behavior with std::memcpy
        }

        const auto num_bytes = sizeof(U) * reader_element_count;
        const auto remaining_bytes = bytes() - offset() * sizeof(T);

        if (remaining_bytes < num_bytes)
        {
            extend((num_bytes - remaining_bytes) / sizeof(T));
        }

        std::memcpy(data_->data() + offset_, reader.data() + reader.offset(), num_bytes);
        offset_ += num_bytes / sizeof(T);
    }

private:
    std::vector<T> *data_ = nullptr;
    std::size_t offset_ = 0;
};

// append_NUL specifies whether the NUL terminator should be added to the binary representation.
template<typename T>
std::vector<byte> string_to_bytes(const std::basic_string<T> &string, bool append_NUL)
{
    std::vector<byte> bytes;
    binary_writer<byte> writer(bytes);
    writer.assign(string, append_NUL);

    return bytes;
}

template<typename T>
T read(std::istream &in)
{
    T result;
    in.read(reinterpret_cast<char *>(&result), sizeof(T));

    return result;
}

template<typename T>
std::vector<T> read_vector(std::istream &in, std::size_t count)
{
    std::vector<T> result(count, T());
    in.read(reinterpret_cast<char *>(&result[0]),
        static_cast<std::streamsize>(sizeof(T) * count));

    return result;
}

/// contains_NUL specifies whether the count contains the NUL terminator. In that case,
/// the NUL terminator will be read from the stream, but will be removed before the string will be returned.
template<typename T>
std::basic_string<T> read_string(std::istream &in, std::size_t count, bool contains_NUL)
{
    std::basic_string<T> result(count, T());
    in.read(reinterpret_cast<char *>(&result[0]),
        static_cast<std::streamsize>(sizeof(T) * count));

    if (contains_NUL)
    {
        result.pop_back();
    }

    return result;
}

} // namespace detail
} // namespace xlnt
