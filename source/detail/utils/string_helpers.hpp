// Copyright (c) 2024-2025 xlnt-community
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

#include <string>
#include <vector>

// If available, allow using C++20 feature test macros for precise feature testing. Useful for compilers
// that partially implement certain features.
#ifdef __has_include
# if __has_include(<version>)
#   include <version>
# endif
#endif

#ifdef __has_include
# if __has_include(<string_view>)
#   include <string_view>
# endif
#endif

#include <detail/xlnt_config_impl.hpp>

namespace xlnt {
namespace detail {

#define LSTRING_LITERAL2(a) L##a
#define U8STRING_LITERAL2(a) u8##a
#define U16STRING_LITERAL2(a) u16##a
#define U32STRING_LITERAL2(a) u32##a
#define LSTRING_LITERAL(a) LSTRING_LITERAL2(a)
#define U8STRING_LITERAL(a) U8STRING_LITERAL2(a)
#define U16STRING_LITERAL(a) U16STRING_LITERAL2(a)
#define U32STRING_LITERAL(a) U32STRING_LITERAL2(a)

#ifdef __cpp_char8_t
// For C++20 and newer, interpret as UTF-8 and then cast to string literal
#define U8_CAST_CONST_LITERAL(a) xlnt::detail::to_const_char_ptr(a)
#define U8_CAST_LITERAL(a) xlnt::detail::to_char_ptr(a)
#else
// For C++11, C++14 and C++17, simply interpret as UTF-8, which works with classic string literals.
#define U8_CAST_CONST_LITERAL(a) a
#define U8_CAST_LITERAL(a) a
#endif

#ifdef __cpp_char8_t
/// Casts const char8_t arrays from C++20 to const char arrays.
inline const char * to_const_char_ptr(const char8_t *utf8)
{
    return reinterpret_cast<const char *>(utf8);
}

/// Casts char8_t arrays from C++20 to char arrays.
inline char * to_char_ptr(char8_t *utf8)
{
    return reinterpret_cast<char *>(utf8);
}
#endif

#ifdef __cpp_lib_char8_t
/// Casts std::u8string_view from C++20 to std::string_view.
inline std::string_view to_string_view(std::u8string_view utf8)
{
    return std::string_view{to_const_char_ptr(utf8.data()), utf8.length()};
}

/// Copies std::u8string(_view) from C++20 to std::string.
inline std::string to_string_copy(std::u8string_view utf8)
{
    return std::string{utf8.begin(), utf8.end()};
}
#endif

/// <summary>
/// Return a vector containing string split at each delim.
/// If the input string is empty, an empty vector is returned.
/// </summary>
XLNT_API_INTERNAL std::vector<std::string> split_string(const std::string &string, char delim);

/// <summary>
/// Concatenate all the provided items by converting them to a string using its to_string member function.
/// </summary>
template<typename T>
XLNT_API_INTERNAL std::string join(const std::vector<T> &items, char delim)
{
    std::string refs;
    for (const auto& item : items)
    {
        if (!refs.empty())
            refs.push_back(delim);

        refs.append(item.to_string());
    }

    return refs;
}

} // namespace detail
} // namespace xlnt
