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

#include <detail/parsers.hpp>
#include <detail/locale.hpp>

#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <limits>
#include <locale>

bool xlnt::detail::parse(const char *string, long long &result, char **end, int base)
{
    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        char *internal_end = nullptr;
        const auto parsed = std::strtoll(string, &internal_end, base);

        if (end != nullptr)
        {
            *end = internal_end;
        }

        // Do something similar to stoll's std::invalid_argument exception.
        if (string == internal_end)
        {
            ok = false;
        }

        // Do something similar to stoll's std::out_of_range exception.
        if (errno_ref == ERANGE)
        {
            ok = false;
        }

        if (ok)
        {
            result = parsed;
        }
    }

    return ok;
}

bool xlnt::detail::parse(const char *string, int &result, char **end, int base)
{
    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        char *internal_end = nullptr;
        const auto parsed = std::strtol(string, &internal_end, base);

        if (end != nullptr)
        {
            *end = internal_end;
        }

        // Do something similar to stoi's std::invalid_argument exception.
        if (string == internal_end)
        {
            ok = false;
        }

        // Do something similar to stoi's std::out_of_range exception.
        if (errno_ref == ERANGE)
        {
            ok = false;
        }

        // Special case: for some reason, the C Standard Library does not allow parsing "int" values,
        // so on platforms where "long" is 64-bit (Linux, macOS, but NOT Windows),
        // we must additionally check whether our "long" is in the "int" range.
        if (parsed < std::numeric_limits<int>::min() || parsed > std::numeric_limits<int>::max())
        {
            // Set errno to ERANGE, which is what strtol would do on under/overflow.
            errno_ref = ERANGE;
            ok = false;
        }

        if (ok)
        {
            result = static_cast<int>(parsed);
        }
    }

    return ok;
}

bool xlnt::detail::parse(const char *string, long &result, char **end, int base)
{
    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        char *internal_end = nullptr;
        const auto parsed = std::strtol(string, &internal_end, base);

        if (end != nullptr)
        {
            *end = internal_end;
        }

        // Do something similar to stol's std::invalid_argument exception.
        if (string == internal_end)
        {
            ok = false;
        }

        // Do something similar to stol's std::out_of_range exception.
        if (errno_ref == ERANGE)
        {
            ok = false;
        }

        if (ok)
        {
            result = parsed;
        }
    }

    return ok;
}

bool xlnt::detail::parse(const std::string &string, long long &result, std::size_t *num_characters_parsed, int base)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(string.c_str(), result, &end, base);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const std::string &string, int &result, std::size_t *num_characters_parsed, int base)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(string.c_str(), result, &end, base);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const std::string &string, long &result, std::size_t *num_characters_parsed, int base)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(string.c_str(), result, &end, base);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::is_negative_number(const char *string, const char *&end)
{
    bool is_negative_number = false;
    end = string;

    // Special case: std::stoul does NOT handle the minus sign properly (the C specification makes no sense here),
    // so -1 becomes ULONG_MAX - 1 instead of failing. However, we want it to fail instead, so let's handle it properly.
    // Skip spaces and check whether the number begins with a minus.
    for (auto *current = string; *current != '\0'; ++current)
    {
        // We must use "unsigned char" to avoid undefined behaviour.
        if (!std::isspace(static_cast<unsigned char>(*current)))
        {
            // Handle the case: -5 is not allowed and out of range
            const bool minus_found = *current == '-';

            // However, it's only out of range if the next character after the minus is a digit (otherwise the parsing failed but NOT because of out of range errors).
            // Note: spaces after the minus don't have to be handled, as std::stoul doesn't support them either.
            if (minus_found && *(current + 1) != '\0')
            {
                const char next_ch = *(current + 1);
                // We must use "unsigned char" to avoid undefined behaviour.
                is_negative_number = std::isdigit(static_cast<unsigned char>(next_ch));

                // Tell the caller that we stopped at the minus.
                if (is_negative_number)
                {
                    end = current;
                }
            }

            break;
        }
    }

    return is_negative_number;
}

bool xlnt::detail::parse(const char *string, unsigned long long &result, char **end, int base)
{
    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        // Special case: std::strtoull does NOT handle the minus sign properly (the C specification makes no sense here),
        // so -1 becomes ULLONG_MAX - 1 instead of failing. However, we want it to fail instead, so let's handle it properly.
        // Skip spaces and check whether the number begins with a minus.
        const char *negative_number_check_end = nullptr;
        ok = !is_negative_number(string, negative_number_check_end);

        if (!ok)
        {
            // Set errno to ERANGE, which is what strtoull would do on under/overflow.
            errno_ref = ERANGE;

            if (end != nullptr)
            {
                // The great C Standard Library is not const correct, so we can't be either... ugh. Note that you are NOT allowed to modify
                // the pointer to the end (thus modifying the string) without resulting in undefined behaviour!
                *end = const_cast<char *>(negative_number_check_end);
            }
        }

        if (ok)
        {
            char *internal_end = nullptr;
            const auto parsed = std::strtoull(string, &internal_end, base);

            if (end != nullptr)
            {
                *end = internal_end;
            }

            // Do something similar to stoull's std::invalid_argument exception.
            if (string == internal_end)
            {
                ok = false;
            }

            // Do something similar to stoull's std::out_of_range exception.
            if (errno_ref == ERANGE)
            {
                ok = false;
            }

            if (ok)
            {
                result = parsed;
            }
        }
    }

    return ok;
}

bool xlnt::detail::parse(const char *string, unsigned int &result, char **end, int base)
{
    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        // Special case: std::strtoul does NOT handle the minus sign properly (the C specification makes no sense here),
        // so -1 becomes ULONG_MAX - 1 instead of failing. However, we want it to fail instead, so let's handle it properly.
        // Skip spaces and check whether the number begins with a minus.
        const char *negative_number_check_end = nullptr;
        ok = !is_negative_number(string, negative_number_check_end);

        if (!ok)
        {
            // Set errno to ERANGE, which is what strtoul would do on under/overflow.
            errno_ref = ERANGE;

            if (end != nullptr)
            {
                // The great C Standard Library is not const correct, so we can't be either... ugh. Note that you are NOT allowed to modify
                // the pointer to the end (thus modifying the string) without resulting in undefined behaviour!
                *end = const_cast<char *>(negative_number_check_end);
            }
        }

        if (ok)
        {
            char *internal_end = nullptr;
            const auto parsed = std::strtoul(string, &internal_end, base);

            if (end != nullptr)
            {
                *end = internal_end;
            }

            // Do something similar to stoul's std::invalid_argument exception.
            if (string == internal_end)
            {
                ok = false;
            }

            // Do something similar to stoul's std::out_of_range exception.
            if (errno_ref == ERANGE)
            {
                ok = false;
            }

            // Special case: for some reason, the C Standard Library does not allow parsing "unsigned int" values,
            // so on platforms where "unsigned long" is 64-bit (Linux, macOS, but NOT Windows),
            // we must additionally check whether our "unsigned long" is in the "unsigned int" range.
            if (parsed > std::numeric_limits<unsigned int>::max())
            {
                // Set errno to ERANGE, which is what strtoul would do on under/overflow.
                errno_ref = ERANGE;
                ok = false;
            }

            if (ok)
            {
                result = static_cast<unsigned int>(parsed);
            }
        }
    }

    return ok;
}

bool xlnt::detail::parse(const char *string, unsigned long &result, char **end, int base)
{
    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        // Special case: std::strtoul does NOT handle the minus sign properly (the C specification makes no sense here),
        // so -1 becomes ULONG_MAX - 1 instead of failing. However, we want it to fail instead, so let's handle it properly.
        // Skip spaces and check whether the number begins with a minus.
        const char *negative_number_check_end = nullptr;
        ok = !is_negative_number(string, negative_number_check_end);

        if (!ok)
        {
            // Set errno to ERANGE, which is what strtoul would do on under/overflow.
            errno_ref = ERANGE;

            if (end != nullptr)
            {
                // The great C Standard Library is not const correct, so we can't be either... ugh. Note that you are NOT allowed to modify
                // the pointer to the end (thus modifying the string) without resulting in undefined behaviour!
                *end = const_cast<char *>(negative_number_check_end);
            }
        }

        if (ok)
        {
            char *internal_end = nullptr;
            const auto parsed = std::strtoul(string, &internal_end, base);

            if (end != nullptr)
            {
                *end = internal_end;
            }

            // Do something similar to stoul's std::invalid_argument exception.
            if (string == internal_end)
            {
                ok = false;
            }

            // Do something similar to stoul's std::out_of_range exception.
            if (errno_ref == ERANGE)
            {
                ok = false;
            }

            if (ok)
            {
                result = parsed;
            }
        }
    }

    return ok;
}

bool xlnt::detail::parse(const std::string &string, unsigned long long &result, std::size_t *num_characters_parsed, int base)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(string.c_str(), result, &end, base);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const std::string &string, unsigned int &result, std::size_t *num_characters_parsed, int base)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(string.c_str(), result, &end, base);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const std::string &string, unsigned long &result, std::size_t *num_characters_parsed, int base)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(string.c_str(), result, &end, base);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const char *string, double &result, char **end)
{
    return parse(detail::get_system_locale(), string, result, end);
}

bool xlnt::detail::parse(const char *string, float &result, char **end)
{
    return parse(detail::get_system_locale(), string, result, end);
}

bool xlnt::detail::parse(const char *string, long double &result, char **end)
{
    return parse(detail::get_system_locale(), string, result, end);
}

// The following function must be "static" to avoid noexcept-type warnings / errors on GCC. See https://stackoverflow.com/a/46857525
template <typename T, typename ParseFunc>
static bool parse_number(const std::locale &loc, const char *string, T &result, char **end, ParseFunc func)
{
    // Only accept float, double and long double, but NOT any other floating-point types (e.g. extended floating-point types)
    // since they are NOT supported by the C parsers!
    static_assert(std::is_same<float, typename std::remove_cv<T>::type>::value
        || std::is_same<double, typename std::remove_cv<T>::type>::value
        || std::is_same<long double, typename std::remove_cv<T>::type>::value,
        "only float, double and long double can be parsed by the C parsers");

    auto &errno_ref = errno; // Nonzero cost, pay it once; note: since c++11, errno is thread-local and thus safe
    errno_ref = 0;

    bool ok = string != nullptr;
    assert(ok); // accepting nullptr strings hides bugs

    if (ok)
    {
        // Note: caching the decimal separator used by the parsing functions can cause thread-safety issues in multi-threaded programs. On the other hand,
        // this would also mean that the rest of this function's logic would be wrong in such cases. Since this isn't something we can fix in a thread-safe way,
        // because we cannot control how users of our library call locale-dependent functions, this is a risk we have to take.
        const char parser_decimal_separator = localeconv()->decimal_point[0];
        const char decimal_separator = std::use_facet<std::numpunct<char>>(loc).decimal_point();
        bool tried_dot = false;
        bool tried_comma = false;

        // Step 1: try to parse the number as it is.
        char *internal_end = nullptr;
        T parsed = func(string, &internal_end);

        if (parser_decimal_separator == '.')
        {
            tried_dot = true;
        }
        else if (parser_decimal_separator == ',')
        {
            tried_comma = true;
        }

        // Step 2: if the string was only partially parsed, try to replace the decimal separator by the one from the provided locale.
        if (*internal_end == decimal_separator && parser_decimal_separator != decimal_separator)
        {
            std::string replaced(string);
            std::replace(replaced.begin(), replaced.end(), decimal_separator, parser_decimal_separator);
            parsed = func(replaced.c_str(), &internal_end);

            if (decimal_separator == '.')
            {
                assert(!tried_dot);
                tried_dot = true;
            }
            else if (decimal_separator == ',')
            {
                assert(!tried_comma);
                tried_comma = true;
            }

            // The end has to point to the end of the original string, not the replaced one.
            // The great C Standard Library is not const correct, so we can't be either... ugh. Note that you are NOT allowed to modify
            // the pointer to the end (thus modifying the string) without resulting in undefined behaviour!
            internal_end = const_cast<char *>(string + (internal_end - replaced.c_str()));
        }

        // Step 3: if the string was only partially parsed even when using the provided locale,
        // maybe the locale has the wrong decimal separator and . was the decimal separator?
        if (*internal_end == '.' && !tried_dot)
        {
            assert(parser_decimal_separator != '.');
            std::string replaced(string);
            std::replace(replaced.begin(), replaced.end(), '.', parser_decimal_separator);
            parsed = func(replaced.c_str(), &internal_end);

            tried_dot = true;

            // The end has to point to the end of the original string, not the replaced one.
            // The great C Standard Library is not const correct, so we can't be either... ugh. Note that you are NOT allowed to modify
            // the pointer to the end (thus modifying the string) without resulting in undefined behaviour!
            internal_end = const_cast<char *>(string + (internal_end - replaced.c_str()));
        }

        // Step 4: if the string was only partially parsed even when using the provided locale,
        // maybe the locale has the wrong decimal separator and , was the decimal separator?
        if (*internal_end == ',' && !tried_comma)
        {
            assert(parser_decimal_separator != ',');
            std::string replaced(string);
            std::replace(replaced.begin(), replaced.end(), ',', parser_decimal_separator);
            parsed = func(replaced.c_str(), &internal_end);

            tried_comma = true;

            // The end has to point to the end of the original string, not the replaced one.
            // The great C Standard Library is not const correct, so we can't be either... ugh. Note that you are NOT allowed to modify
            // the pointer to the end (thus modifying the string) without resulting in undefined behaviour!
            internal_end = const_cast<char *>(string + (internal_end - replaced.c_str()));
        }

        if (end != nullptr)
        {
            *end = internal_end;
        }

        // Do something similar to stod's std::invalid_argument exception.
        if (string == internal_end)
        {
            ok = false;
        }

        // Do something similar to stod's std::out_of_range exception.
        if (errno_ref == ERANGE)
        {
            ok = false;
        }

        if (ok)
        {
            result = parsed;
        }
    }

    return ok;
}

bool xlnt::detail::parse(const std::locale &loc, const char *string, double &result, char **end)
{
    return ::parse_number<double>(loc, string, result, end, std::strtod);
}

bool xlnt::detail::parse(const std::locale &loc, const char *string, float &result, char **end)
{
    return ::parse_number<float>(loc, string, result, end, std::strtof);
}

bool xlnt::detail::parse(const std::locale &loc, const char *string, long double &result, char **end)
{
    return ::parse_number<long double>(loc, string, result, end, std::strtold);
}

bool xlnt::detail::parse(const std::string &string, double &result, std::size_t *num_characters_parsed)
{
    return parse(detail::get_system_locale(), string, result, num_characters_parsed);
}

bool xlnt::detail::parse(const std::string &string, float &result, std::size_t *num_characters_parsed)
{
    return parse(detail::get_system_locale(), string, result, num_characters_parsed);
}

bool xlnt::detail::parse(const std::string &string, long double &result, std::size_t *num_characters_parsed)
{
    return parse(detail::get_system_locale(), string, result, num_characters_parsed);
}

bool xlnt::detail::parse(const std::locale &loc, const std::string &string, double &result, std::size_t *num_characters_parsed)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(loc, string.c_str(), result, &end);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const std::locale &loc, const std::string &string, float &result, std::size_t *num_characters_parsed)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(loc, string.c_str(), result, &end);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}

bool xlnt::detail::parse(const std::locale &loc, const std::string &string, long double &result, std::size_t *num_characters_parsed)
{
    errno = 0; // Reset errno; note: since c++11, errno is thread-local and thus safe
    bool ok = !string.empty();

    if (ok)
    {
        char *end = nullptr;
        ok = parse(loc, string.c_str(), result, &end);

        if (num_characters_parsed != nullptr)
        {
            *num_characters_parsed = end - string.c_str();
        }
    }
    else if (num_characters_parsed != nullptr)
    {
        *num_characters_parsed = 0;
    }

    return ok;
}