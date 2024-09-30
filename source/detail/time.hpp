// Copyright (c) 2024 xlnt-community
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

#include <ctime>

namespace xlnt {
namespace detail {

inline std::tm localtime_safe(std::time_t raw_time)
{
    std::tm result{};

#ifdef _MSC_VER
    localtime_s(&result, &raw_time);
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
    localtime_r(&raw_time, &result);
#else
    std::tm *tm = std::localtime(&raw_time);

    if (tm != nullptr)
    {
        result = *tm;
    }
#endif

    return result;
}

inline std::tm gmtime_safe(std::time_t raw_time)
{
    std::tm result{};

#ifdef _MSC_VER
    gmtime_s(&result, &raw_time);
#elif _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
    gmtime_r(&raw_time, &result);
#else
    std::tm *tm = std::gmtime(&raw_time);

    if (tm != nullptr)
    {
        result = *tm;
    }
#endif

    return result;
}

} // namespace detail
} // namespace xlnt