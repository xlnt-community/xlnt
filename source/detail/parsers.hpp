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

#include <string>
#include <locale>
#include <xlnt/xlnt_config.hpp>

/// The following parsers offer the same convenience as C++'s parsers (std::stoll, std::stod, etc.) but do NOT require exception handling,
/// resulting in improved performance and convenience to the programmer (exception handling is not required everywhere).
namespace xlnt {
namespace detail {

/// ----- (SIGNED) INTEGER PARSING -----

XLNT_API bool parse(const char *string, long long &result, char **end = nullptr, int base = 10);
XLNT_API bool parse(const char *string, int &result, char **end = nullptr, int base = 10);
XLNT_API bool parse(const char *string, long &result, char **end = nullptr, int base = 10);

XLNT_API bool parse(const std::string &string, long long &result, std::size_t *num_characters_parsed = nullptr, int base = 10);
XLNT_API bool parse(const std::string &string, int &result, std::size_t *num_characters_parsed = nullptr, int base = 10);
XLNT_API bool parse(const std::string &string, long &result, std::size_t *num_characters_parsed = nullptr, int base = 10);


/// ----- UNSIGNED INTEGER PARSING -----

/// Special case: std::stoul does NOT handle the minus sign properly (the C specification makes no sense here),
/// so -1 becomes ULONG_MAX - 1 instead of failing. However, we want it to fail instead, so let's handle it properly.
XLNT_API bool is_negative_number(const char *string, const char *&end);

XLNT_API bool parse(const char *string, unsigned long long &result, char **end = nullptr, int base = 10);
XLNT_API bool parse(const char *string, unsigned int &result, char **end = nullptr, int base = 10);
XLNT_API bool parse(const char *string, unsigned long &result, char **end = nullptr, int base = 10);

XLNT_API bool parse(const std::string &string, unsigned long long &result, std::size_t *num_characters_parsed = nullptr, int base = 10);
XLNT_API bool parse(const std::string &string, unsigned int &result, std::size_t *num_characters_parsed = nullptr, int base = 10);
XLNT_API bool parse(const std::string &string, unsigned long &result, std::size_t *num_characters_parsed = nullptr, int base = 10);


/// ----- FLOATING-POINT NUMBER PARSING -----

/// Parse a floating-point number using the system locale (considering the user's system settings).
XLNT_API bool parse(const char *string, double &result, char **end = nullptr);
XLNT_API bool parse(const char *string, float &result, char **end = nullptr);
XLNT_API bool parse(const char *string, long double &result, char **end = nullptr);

/// Parse a floating-point number using the provided locale.
XLNT_API bool parse(const std::locale &loc, const char *string, double &result, char **end = nullptr);
XLNT_API bool parse(const std::locale &loc, const char *string, float &result, char **end = nullptr);
XLNT_API bool parse(const std::locale &loc, const char *string, long double &result, char **end = nullptr);

/// Parse a floating-point number using the system locale (considering the user's system settings).
XLNT_API bool parse(const std::string &string, double &result, std::size_t *num_characters_parsed = nullptr);
XLNT_API bool parse(const std::string &string, float &result, std::size_t *num_characters_parsed = nullptr);
XLNT_API bool parse(const std::string &string, long double &result, std::size_t *num_characters_parsed = nullptr);

/// Parse a floating-point number using the provided locale.
XLNT_API bool parse(const std::locale &loc, const std::string &string, double &result, std::size_t *num_characters_parsed = nullptr);
XLNT_API bool parse(const std::locale &loc, const std::string &string, float &result, std::size_t *num_characters_parsed = nullptr);
XLNT_API bool parse(const std::locale &loc, const std::string &string, long double &result, std::size_t *num_characters_parsed = nullptr);

} // namespace detail
} // namespace xlnt