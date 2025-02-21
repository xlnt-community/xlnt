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

#include <detail/xlnt_config_impl.hpp>
#include <xlnt/internal/features.hpp>

#if XLNT_HAS_INCLUDE(<string_view>) && XLNT_HAS_FEATURE(U8_STRING_VIEW)
  #include <string_view>
#endif

namespace xlnt {
namespace detail {

// Prepends the string literal prefix to the provided string literal.
// Useful when defining a string literal once, then using it with multiple string types.
#define LSTRING_LITERAL2(a) L##a
#define U8STRING_LITERAL2(a) u8##a
#define U16STRING_LITERAL2(a) u##a
#define U32STRING_LITERAL2(a) U##a
#define LSTRING_LITERAL(a) LSTRING_LITERAL2(a)
#define U8STRING_LITERAL(a) U8STRING_LITERAL2(a)
#define U16STRING_LITERAL(a) U16STRING_LITERAL2(a)
#define U32STRING_LITERAL(a) U32STRING_LITERAL2(a)

// Casts a UTF-8 string literal to a narrow string literal without changing its encoding.
#ifdef __cpp_char8_t
// For C++20 and newer, interpret as UTF-8 and then cast to string literal
#define U8_TO_CHAR_PTR(a) xlnt::detail::to_char_ptr(a)
#else
// For C++11, C++14 and C++17, simply interpret as UTF-8, which works with classic string literals.
#define U8_TO_CHAR_PTR(a) a
#endif


// The following weird cast ensures that the string is UTF-8 encoded at all costs!
#define ENSURE_UTF8_LITERAL(a) U8_TO_CHAR_PTR(U8STRING_LITERAL(a))

#ifdef __cpp_char8_t
/// Casts const char8_t arrays from C++20 to const char arrays.
inline const char * to_char_ptr(const char8_t *utf8)
{
    return reinterpret_cast<const char *>(utf8);
}

/// Casts char8_t arrays from C++20 to char arrays.
inline char * to_char_ptr(char8_t *utf8)
{
    return reinterpret_cast<char *>(utf8);
}
#endif

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
/// Casts std::u8string_view from C++20 to std::string_view.
inline std::string_view to_string_view(std::u8string_view utf8)
{
    return std::string_view{to_char_ptr(utf8.data()), utf8.length()};
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
std::string join(const std::vector<T> &items, char delim)
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
