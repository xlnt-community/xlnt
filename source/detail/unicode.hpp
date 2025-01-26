// Copyright (c) 2014-2022 Thomas Fussell
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

namespace xlnt {
namespace detail {

std::u16string utf8_to_utf16(const std::string &utf8_string);
std::u32string utf8_to_utf32(const std::string &utf8_string);
std::string utf16_to_utf8(const std::u16string &utf16_string);
std::string utf32_to_utf8(const std::u32string &utf32_string);
std::string latin1_to_utf8(const std::string &latin1);
size_t string_length(const std::string &utf8_string);

#ifdef __cpp_lib_char8_t
std::u16string utf8_to_utf16(std::u8string_view utf8_string);
std::u32string utf8_to_utf32(std::u8string_view utf8_string);
std::u8string utf16_to_utf8(std::u16string_view utf16_string);
std::u8string utf32_to_utf8(std::u32string_view utf32_string);
#endif

} // namespace detail
} // namespace xlnt
