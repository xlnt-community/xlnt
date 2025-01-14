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

#include <detail/xlnt_config_impl.hpp>
#include <locale>
#include <string>

namespace xlnt {
namespace detail {

// Namespace internal for functions that should not be called directly (unless you have a good reason).
namespace internal {
    /// Returns the non-cached current system locale of the PC.
    /// Note: getting the current system locale very often is VERY expensive! Prefer using the caching function whenever possible!
    inline std::locale get_system_locale_NOT_cached()
    {
        return std::locale{""}; // the C++ standard specifies that using an empty string will create the system locale
    }
} // namespace internal

/// Returns the current system locale of the PC. If it has previously been determined, it returns the cached system locale,
/// otherwise searches for the current system locale and caches it (for performance reasons). In other words, if the PC's locale changes during runtime,
/// this function will return the original system locale.
inline const std::locale &get_system_locale()
{
    static const std::locale system_loc = internal::get_system_locale_NOT_cached();
    return system_loc;
}

inline const std::locale &get_serialization_locale()
{
    return std::locale::classic();
}

/// Returns the locale-specific decimal separator. This function also accounts
/// for the case where certain decimal separators might not be able to be represented
/// as single chars in some locales.
XLNT_API_INTERNAL std::string get_locale_decimal_separator(const std::locale &loc);

/// Converts a single wchar_t to an std::string, with a fallback char for errors.
/// Useful when certain characters cannot be represented as char (due to multibyte encodings like UTF-8),
/// but can be represented as wchar_t (which uses UTF-16 on Windows and UTF-32 on Linux/macOS).
XLNT_API_INTERNAL std::string wide_char_to_str(const std::locale &loc, wchar_t wide, char fallback_for_errors);

} // namespace detail
} // namespace xlnt
