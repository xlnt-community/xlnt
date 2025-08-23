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
            // to correctly run this test *and* make sure that the locale uses the expected decimal separator,
            // or alternatively disable the CMake option XLNT_USE_LOCALE_****_DECIMAL_SEPARATOR.
            throw xlnt::invalid_parameter(error);
        }

    }
    ~SetLocale() {std::setlocale(LC_ALL, previous_locale);}

    char * previous_locale = nullptr;
};

} // namespace test_helpers
