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

#include <memory>
#include <string>
#include <vector>
#include <xlnt/xlnt_config.hpp>
#include <detail/xlnt_config_impl.hpp>
#include <xlnt/utils/path.hpp>

namespace xlnt {
namespace detail {

/// Abstract interface for reading ZIP archives.
///
/// Lifetime contract:
///   The archive_reader instance must not outlive the std::istream
///   passed to the factory function that created it.
///
/// Thread-safety:
///   archive_reader is not thread-safe; do not call methods concurrently on the same instance.
///   Whether streambufs returned by open() can be read concurrently depends on the backend.
///   The builtin backend (miniz/Partio-based) shares the underlying istream and does NOT guarantee
///   safe concurrent reading across multiple streambufs.
class XLNT_API_INTERNAL archive_reader {
public:
    virtual ~archive_reader() = default;

    /// Open a file in the archive for reading.
    /// Returns: A streambuf positioned at the start of the uncompressed data.
    ///          The streambuf supports forward sequential reading only (no seeking).
    /// Throws: xlnt::exception if file not found or decompression fails.
    virtual std::unique_ptr<std::streambuf> open(const path &file) const = 0;

    /// Read entire file content as a string (convenience wrapper).
    /// Throws: xlnt::exception if file not found or decompression fails.
    virtual std::string read(const path &file) const = 0;

    /// Get list of all files in the archive.
    /// Returns: List of paths. Backends should return central-directory order when possible.
    ///          The builtin backend returns the order recorded while reading the central directory.
    virtual std::vector<path> files() const = 0;

    /// Check if a file exists in the archive.
    /// Note: Comparison is case-sensitive, paths use '/' separator (POSIX style).
    virtual bool has_file(const path &file) const = 0;
};

/// Abstract interface for writing ZIP archives.
///
/// Lifetime contract:
///   The archive_writer instance must not outlive the std::ostream
///   passed to the factory function that created it.
///
/// Finalization:
///   The central directory is written when the writer is destroyed.
///   If an exception is thrown during destruction, the archive may be incomplete.
///
/// Thread-safety:
///   archive_writer is NOT thread-safe.
class XLNT_API_INTERNAL archive_writer {
public:
    virtual ~archive_writer() = default;

    /// Open a file for writing in the archive.
    /// Returns: A streambuf for writing uncompressed data.
    ///          The file entry is finalized when the streambuf is destroyed.
    virtual std::unique_ptr<std::streambuf> open(const path &file) = 0;
};

} // namespace detail
} // namespace xlnt
