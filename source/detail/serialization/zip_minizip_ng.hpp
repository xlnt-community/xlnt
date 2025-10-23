// Copyright (c) 2025 xlnt-community
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

#ifdef XLNT_USE_MINIZIP_NG

#include <detail/serialization/archive.hpp>
#include <streambuf>
#include <unordered_map>
#include <vector>

// Avoid including minizip headers in this header to keep compile units light

namespace xlnt {
namespace detail {

// Forward declaration of custom iostream wrapper used by the minizip stream
struct mz_stream_iostream;

/// Path normalization for ZIP archives
/// Converts Windows paths to POSIX, ensures UTF-8 encoding
std::string normalize_zip_path(const xlnt::path &p);

/// minizip-ng based ZIP reader (supports Zip64)
class zip_minizip_reader : public archive_reader {
public:
    explicit zip_minizip_reader(std::istream &stream);
    ~zip_minizip_reader() override;

    // Delete copy/move (holds non-copyable handle)
    zip_minizip_reader(const zip_minizip_reader&) = delete;
    zip_minizip_reader& operator=(const zip_minizip_reader&) = delete;

    std::unique_ptr<std::streambuf> open(const path &file) const override;
    std::string read(const path &file) const override;
    std::vector<path> files() const override;
    bool has_file(const path &file) const override;

    // Entry open tracking (called by minizip_streambuf)
    void mark_entry_opened() const;
    void mark_entry_closed() const;

private:
    void build_file_index();

    std::istream &source_stream_;
    void *zip_handle_;  // Actually mz_zip*, but avoid header dependency
    void *stream_handle_ = nullptr; // mz_stream* used to open
    mz_stream_iostream *ios_stream_ = nullptr;
    mutable std::unordered_map<std::string, int64_t> file_index_;  // filename -> entry index
    mutable std::vector<std::string> file_order_;  // Stable ordering (central directory order)
    mutable bool index_built_;
    mutable bool entry_open_;  // Track if an entry is currently open
};

/// Streambuf wrapper for minizip-ng file entry (read-only)
class minizip_streambuf : public std::streambuf {
public:
    minizip_streambuf(void *zip_handle, const std::string &filename, zip_minizip_reader *reader);
    ~minizip_streambuf() override;

    // Delete copy/move
    minizip_streambuf(const minizip_streambuf&) = delete;
    minizip_streambuf& operator=(const minizip_streambuf&) = delete;

protected:
    std::streambuf::int_type underflow() override;

private:
    void *zip_handle_;
    zip_minizip_reader *reader_;  // For entry tracking
    std::vector<char> buffer_;
    static constexpr size_t buffer_size = 65536;  // 64KB for better performance
    bool eof_reached_;
};

/// minizip-ng based ZIP writer (supports Zip64)
class zip_minizip_writer : public archive_writer {
public:
    explicit zip_minizip_writer(std::ostream &stream);
    ~zip_minizip_writer() override;

    // Delete copy/move
    zip_minizip_writer(const zip_minizip_writer&) = delete;
    zip_minizip_writer& operator=(const zip_minizip_writer&) = delete;

    std::unique_ptr<std::streambuf> open(const path &file) override;

    // Entry open tracking (called by minizip_write_streambuf)
    void mark_entry_opened();
    void mark_entry_closed();

private:
    std::ostream &destination_stream_;
    void *zip_handle_;  // Actually mz_zip*
    void *stream_handle_ = nullptr; // mz_stream* used to open
    mz_stream_iostream *ios_stream_ = nullptr;
    bool entry_open_;  // Track if an entry is currently open
};

/// Streambuf for writing to minizip-ng
class minizip_write_streambuf : public std::streambuf {
public:
    minizip_write_streambuf(void *zip_handle, const std::string &filename, zip_minizip_writer *writer);
    ~minizip_write_streambuf() override;

    // Delete copy/move
    minizip_write_streambuf(const minizip_write_streambuf&) = delete;
    minizip_write_streambuf& operator=(const minizip_write_streambuf&) = delete;

protected:
    std::streambuf::int_type overflow(std::streambuf::int_type c) override;
    int sync() override;
    std::streamsize xsputn(const char* s, std::streamsize n) override;

private:
    void flush_buffer();

    void *zip_handle_;
    zip_minizip_writer *writer_;  // For entry tracking
    std::vector<char> buffer_;
    static constexpr size_t buffer_size = 65536;  // 64KB for better performance
};

} // namespace detail
} // namespace xlnt

#endif // XLNT_USE_MINIZIP_NG
