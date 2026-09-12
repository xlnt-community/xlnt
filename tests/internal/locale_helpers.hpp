// Copyright (c) 2024-2026 xlnt-community
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

// Sets both the C locale (as seen by e.g. setlocale/localeconv) and the *global C++ locale*
// (std::locale::global), which is what a default-constructed std::ostringstream is imbued with.
// Setting both is required to reproduce bugs caused by code (such as libstudxml's
// default_value_traits<T>::serialize) that formats numbers through such a stream.
struct SetLocale
{
    SetLocale(const char *locale_name, const char *expected_decimal_separator = nullptr,
        const char *expected_thousands_separator = nullptr)
        : previous_locale(setlocale(LC_ALL, nullptr)),
          previous_global_locale(std::locale::global(std::locale(locale_name)))
    {
        xlnt_assert(std::setlocale(LC_ALL, locale_name) != nullptr);

        if (expected_decimal_separator != nullptr
            && strcmp(expected_decimal_separator, localeconv()->decimal_point) != 0)
        {
            std::string error = "Unexpected decimal separator for locale ";
            error += locale_name;
            error += " expected ";
            error += expected_decimal_separator;
            error += " but found ";
            error += localeconv()->decimal_point;

            restore();

            // If failed, please install the locale specified by the CMake variable XLNT_LOCALE_****_DECIMAL_SEPARATOR
            // to correctly run this test *and* make sure that the locale uses the expected decimal separator,
            // or alternatively disable the CMake option XLNT_USE_LOCALE_****_DECIMAL_SEPARATOR.
            throw xlnt::invalid_parameter(error);
        }

        if (expected_thousands_separator != nullptr
            && strcmp(expected_thousands_separator, localeconv()->thousands_sep) != 0)
        {
            std::string error = "Unexpected thousands separator for locale ";
            error += locale_name;
            error += " expected ";
            error += expected_thousands_separator;
            error += " but found ";
            error += localeconv()->thousands_sep;

            restore();

            // If failed, please install the locale specified by the CMake variable XLNT_LOCALE_POINT_THOUSAND_SEPARATOR
            // to correctly run this test *and* make sure that the locale groups digits using the expected
            // thousands separator, or alternatively disable the CMake option XLNT_USE_LOCALE_POINT_THOUSAND_SEPARATOR.
            throw xlnt::invalid_parameter(error);
        }
    }

    ~SetLocale()
    {
        restore();
    }

    char *previous_locale = nullptr;
    std::locale previous_global_locale;

private:
    void restore()
    {
        std::locale::global(previous_global_locale);
        std::setlocale(LC_ALL, previous_locale);
    }
};

} // namespace test_helpers
