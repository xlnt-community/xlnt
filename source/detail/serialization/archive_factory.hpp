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

#include <xlnt/xlnt_config.hpp>
#include <detail/xlnt_config_impl.hpp>
#include <detail/serialization/archive.hpp>
#include <istream>
#include <ostream>

namespace xlnt {
namespace detail {

/// Create an archive reader for the given stream.
/// Backend is selected at compile-time via XLNT_USE_MINIZIP_NG macro.
///
/// Ownership:
///   - Returned unique_ptr owns the reader instance
///   - Reader holds a reference to 'stream' (not ownership)
///   - Caller must ensure 'stream' remains valid for reader's lifetime
XLNT_API_INTERNAL std::unique_ptr<archive_reader> make_archive_reader(std::istream &stream);

/// Create an archive writer for the given stream.
/// Backend is selected at compile-time via XLNT_USE_MINIZIP_NG macro.
///
/// Ownership:
///   - Returned unique_ptr owns the writer instance
///   - Writer holds a reference to 'stream' (not ownership)
///   - Caller must ensure 'stream' remains valid for writer's lifetime
///   - Central directory is written when writer is destroyed
XLNT_API_INTERNAL std::unique_ptr<archive_writer> make_archive_writer(std::ostream &stream);

} // namespace detail
} // namespace xlnt
