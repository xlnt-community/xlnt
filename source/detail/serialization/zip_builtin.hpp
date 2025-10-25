/*
PARTIO SOFTWARE
Copyright 2010 Disney Enterprises, Inc. All rights reserved

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.

* The names "Disney", "Walt Disney Pictures", "Walt Disney Animation
Studios" or the names of its contributors may NOT be used to
endorse or promote products derived from this software without
specific prior written permission from Walt Disney Pictures.

Disclaimer: THIS SOFTWARE IS PROVIDED BY WALT DISNEY PICTURES AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT AND TITLE ARE DISCLAIMED.
IN NO EVENT SHALL WALT DISNEY PICTURES, THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND BASED ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <deque>

#include <xlnt/xlnt_config.hpp>
#include <detail/xlnt_config_impl.hpp>
#include <xlnt/utils/path.hpp>
#include <detail/serialization/archive.hpp>

namespace xlnt {
namespace detail {

/// <summary>
/// A structure representing the header that occurs before each compressed file in a ZIP
/// archive and again at the end of the file with more information.
/// </summary>
struct XLNT_API_INTERNAL zheader
{
    std::uint16_t version = 20;
    std::uint16_t flags = 0;
    std::uint16_t compression_type = 8;
    std::uint16_t stamp_date = 0;
    std::uint16_t stamp_time = 0;
    std::uint32_t crc = 0;
    std::uint32_t compressed_size = 0;
    std::uint32_t uncompressed_size = 0;
    std::string filename;
    std::string comment;
    std::vector<std::uint8_t> extra;
    std::uint32_t header_offset = 0;
};

/// <summary>
/// Builtin ZIP writer using miniz (PARTIO-based implementation).
/// Writes a series of uncompressed binary file data as ostreams into another ostream
/// according to the ZIP format.
/// </summary>
class XLNT_API_INTERNAL zip_builtin_writer : public archive_writer
{
public:
    /// <summary>
    /// Construct a new zip_file_writer which writes a ZIP archive to the given stream.
    /// </summary>
    explicit zip_builtin_writer(std::ostream &stream);

    /// <summary>
    /// Destructor. Writes central directory to stream.
    /// </summary>
    ~zip_builtin_writer() override;

    /// <summary>
    /// Returns a pointer to a streambuf which compresses the data it receives.
    /// </summary>
    std::unique_ptr<std::streambuf> open(const path &file) override;

    /// Forbid multiple simultaneous open entries (matching minizip behavior)
    void mark_entry_opened();
    void mark_entry_closed();

private:
    // Use deque to keep header pointers stable across push_backs while entries are open
    std::deque<zheader> file_headers_;
    std::ostream &destination_stream_;
    bool entry_open_ = false;
};

/// <summary>
/// Builtin ZIP reader using miniz (PARTIO-based implementation).
/// Reads an archive containing a number of files from an istream and allows them
/// to be decompressed into an istream.
/// </summary>
class XLNT_API_INTERNAL zip_builtin_reader : public archive_reader
{
public:
    /// <summary>
    /// Construct a new zip_file_reader which reads a ZIP archive from the given stream.
    /// </summary>
    explicit zip_builtin_reader(std::istream &stream);

    /// <summary>
    /// Destructor.
    /// </summary>
    ~zip_builtin_reader() override;

    /// <summary>
    /// Open a file in the archive for reading.
    /// </summary>
    std::unique_ptr<std::streambuf> open(const path &file) const override;

    /// <summary>
    /// Read entire file content as string.
    /// </summary>
    std::string read(const path &file) const override;

    /// <summary>
    /// Get list of all files in the archive.
    /// </summary>
    std::vector<path> files() const override;

    /// <summary>
    /// Check if a file exists in the archive.
    /// </summary>
    bool has_file(const path &filename) const override;

private:
    /// <summary>
    /// Read the central directory header.
    /// </summary>
    bool read_central_header();

    /// <summary>
    /// Map of filename to header information.
    /// </summary>
    std::unordered_map<std::string, zheader> file_headers_;

    /// <summary>
    /// File list order as read from the central directory, used by files() to return
    /// a stable, deterministic ordering.
    /// </summary>
    std::vector<path> file_order_;

    /// <summary>
    /// Reference to the source stream.
    /// </summary>
    std::istream &source_stream_;
};

} // namespace detail
} // namespace xlnt
