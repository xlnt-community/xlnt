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

#include <helpers/assertions.hpp>
#include <clocale>
#include <cstring>
#include <locale>

namespace test_helpers
{

struct SetLocale
{
    SetLocale(const char* locale_name, const char *expected_decimal_separator)
        : previous_locale(setlocale(LC_ALL, nullptr))
    {
        xlnt_assert(std::setlocale(LC_ALL, locale_name) != nullptr);

        if (strcmp(expected_decimal_separator, localeconv()->decimal_point) != 0)
        {
            std::string error = "Unexpected decimal separator for locale ";
            error += locale_name;
            error += " expected ";
            error += expected_decimal_separator;
            error += " but found ";
            error += localeconv()->decimal_point;

            // If failed, please install the locale specified by the CMake variable XLNT_LOCALE_****_DECIMAL_SEPARATOR
            // to correctly run this test *and* make sure that the locale uses a comma as decimal separator,
            // or alternatively disable the CMake option XLNT_USE_LOCALE_****_DECIMAL_SEPARATOR.
            throw xlnt::invalid_parameter(error.c_str());
        }

    }
    ~SetLocale() {std::setlocale(LC_ALL, previous_locale);}

    char * previous_locale = nullptr;
};

inline std::string wide_char_to_str(const std::locale & loc, wchar_t wide, char fallback_for_errors)
{
    // Note: here we'll use the non-deprecated std::codecvt directly, since std::wstring_convert
    // has been deprecated in C++17 and removed in C++26.
    const auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
    std::mbstate_t state {};
    const wchar_t *input_next = nullptr;
    char *output_next = nullptr;
    std::string conv(facet.max_length(), '\0');
    auto result = facet.out(state, &wide, &wide + 1, input_next, &conv.at(0), &conv.at(0) + conv.length(), output_next);

    if (result == std::codecvt_base::ok)
    {
        conv.resize(output_next - conv.c_str());
    }
    else
    {
        conv.clear();
        conv.push_back(fallback_for_errors);
    }

    return conv;
}

inline std::string get_locale_decimal_separator(const std::locale &loc)
{
    const char narrow = std::use_facet<std::numpunct<char>>(loc).decimal_point();
    const wchar_t wide = std::use_facet<std::numpunct<wchar_t>>(loc).decimal_point();
    return wide_char_to_str(loc, wide, narrow);
}

} // namespace test_helpers
