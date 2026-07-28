// Copyright (c) 2026 xlnt-community
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

#include <ios>

namespace xlnt {
namespace detail {

/// Wrapper for setting and restoring the exception mask of a stream using RAII.
/// Constructing such an object saves the current exception mask and
/// sets the desired exception mask on the object. As soon as the object goes
/// out of scope, the previous exception mask will be restored.
template <typename CharT, typename Traits>
class stream_scoped_exception_mask
{
public:
    explicit stream_scoped_exception_mask(std::basic_ios<CharT,Traits> &stream, std::ios_base::iostate mask)
        : stream_(stream)
        , previous_mask_(stream.exceptions())
    {
        stream_.exceptions(mask);
    }

    ~stream_scoped_exception_mask()
    {
        stream_.exceptions(previous_mask_);
    }

    stream_scoped_exception_mask(const stream_scoped_exception_mask &) = delete;
    stream_scoped_exception_mask &operator=(const stream_scoped_exception_mask &) = delete;
    stream_scoped_exception_mask(stream_scoped_exception_mask &&) = delete;
    stream_scoped_exception_mask &operator=(stream_scoped_exception_mask &&) = delete;

private:
    std::basic_ios<CharT,Traits> &stream_;
    std::ios_base::iostate previous_mask_;
};

} // namespace detail
} // namespace xlnt
