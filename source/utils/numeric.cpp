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

#include <xlnt/utils/numeric.hpp>
#include <detail/locale.hpp>
#include <detail/parsers.hpp>

#include <iomanip>

constexpr auto serialization_digits_full_precision = 15;
constexpr auto serialization_digits_short = 6;

std::string xlnt::detail::number_serialiser::serialise(double d) const
{
    std::ostringstream ss;
    ss.imbue(detail::get_serialization_locale());
    ss << std::defaultfloat << std::setprecision(serialization_digits_full_precision) << d;
    return ss.str();
}

std::string xlnt::detail::number_serialiser::serialise_short(double d) const
{
    std::ostringstream ss;
    ss.imbue(detail::get_serialization_locale());
    ss << std::fixed << std::setprecision(serialization_digits_short) << d;
    return ss.str();
}

double xlnt::detail::number_serialiser::deserialise(const std::string &s, ptrdiff_t *len_converted) const
{
    assert(!s.empty());
    assert(len_converted != nullptr);
    *len_converted = 0;
    double d = std::numeric_limits<double>::quiet_NaN();
    std::size_t num_parsed_characters = 0;
    if (detail::parse(s, d, &num_parsed_characters))
    {
        // Prevent unwanted overflows.
        assert(num_parsed_characters <= std::numeric_limits<ptrdiff_t>::max());
        *len_converted = static_cast<ptrdiff_t>(std::min(num_parsed_characters, static_cast<size_t>(std::numeric_limits<ptrdiff_t>::max())));
    }

    return d;
}