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

#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <limits>
#include <cstring>

namespace xlnt {
namespace detail {

bool parse(const char *string, long long &result, char **end, int base)
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

bool parse(const char *string, int &result, char **end, int base)
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

bool parse(const char *string, long &result, char **end, int base)
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

bool parse(const std::string &string, long long &result, std::size_t *num_characters_parsed, int base)
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

bool parse(const std::string &string, int &result, std::size_t *num_characters_parsed, int base)
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

bool parse(const std::string &string, long &result, std::size_t *num_characters_parsed, int base)
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

bool is_negative_number(const char *string, const char *&end)
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

bool parse(const char *string, unsigned long long &result, char **end, int base)
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

bool parse(const char *string, unsigned int &result, char **end, int base)
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

bool parse(const char *string, unsigned long &result, char **end, int base)
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

bool parse(const std::string &string, unsigned long long &result, std::size_t *num_characters_parsed, int base)
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

bool parse(const std::string &string, unsigned int &result, std::size_t *num_characters_parsed, int base)
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

bool parse(const std::string &string, unsigned long &result, std::size_t *num_characters_parsed, int base)
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

} // namespace detail
} // namespace xlnt
