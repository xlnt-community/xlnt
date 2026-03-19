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

// For compilers implementing the optional Annex K of C11.
// MUST be defined before including <cstring>
// See https://en.cppreference.com/w/c/header/time.html
#if defined(__STDC_LIB_EXT1__) && !defined(__STDC_WANT_LIB_EXT1__)
#    define __STDC_WANT_LIB_EXT1__ 1
#endif


#include "error_helpers.hpp"

#include <cstring>

namespace xlnt {
namespace detail {

std::string strerror_safe(int err_no)
{
    constexpr std::size_t BUFF_SIZE = 8192;
    std::string err_msg;

#if defined(_MSC_VER) || (defined(__STDC_LIB_EXT1__) && defined(__STDC_WANT_LIB_EXT1__))
    char buffer[BUFF_SIZE] = "\0";
    errno_t err = strerror_s(buffer, BUFF_SIZE, err_no);
    if (err == 0)
    {
        err_msg = buffer;
    }

// Feature test macros from https://linux.die.net/man/3/strerror_r and https://linux.die.net/man/7/feature_test_macros
#elif (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && ! _GNU_SOURCE
    char buffer[BUFF_SIZE] = "\0";
    int err = strerror_r(err_no, buffer, BUFF_SIZE);
    if (err == 0)
    {
        err_msg = buffer;
    }

// Feature test macros from https://linux.die.net/man/3/strerror_r and https://linux.die.net/man/7/feature_test_macros
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
    char buffer[BUFF_SIZE] = "\0";
    // The GNU-specific strerror_r() returns a pointer to a string containing the error message.
    // This may be either a pointer to a string that the function stores in buf,
    // or a pointer to some (immutable) static string (in which case buf is unused).
    const char *ret_buf = strerror_r(err_no, buffer, BUFF_SIZE);
    if (buffer[0] != '\0')
    {
        err_msg = buffer;
    }
    else
    {
        err_msg = ret_buf;
    }
#else
    err_msg = strerror(err_no);
#endif

    return err_msg;
}

} // namespace detail
} // namespace xlnt
