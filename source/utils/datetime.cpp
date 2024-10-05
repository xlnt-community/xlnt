// Copyright (c) 2014-2021 Thomas Fussell
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
#include <cmath>
#include <ctime>

#include <xlnt/utils/date.hpp>
#include <xlnt/utils/datetime.hpp>
#include <xlnt/utils/time.hpp>

#include <xlnt/utils/optional.hpp>

namespace {

std::string fill(const std::string &string, std::size_t length = 2)
{
    if (string.size() >= length)
    {
        return string;
    }

    return std::string(length - string.size(), '0') + string;
}

} // namespace

namespace xlnt {

datetime datetime::from_number(double raw_time, calendar base_date)
{
    auto date_part = date::from_number(static_cast<int>(raw_time), base_date);
    auto time_part = time::from_number(raw_time);

    return datetime(date_part, time_part);
}

bool datetime::operator==(const datetime &comparand) const
{
    return year == comparand.year
        && month == comparand.month
        && day == comparand.day
        && hour == comparand.hour
        && minute == comparand.minute
        && second == comparand.second
        && microsecond == comparand.microsecond
        && _is_null == comparand._is_null;
}

double datetime::to_number(calendar base_date) const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("cannot convert invalid/empty datetime to a number");
    }

    return date(year, month, day).to_number(base_date)
        + time(hour, minute, second, microsecond).to_number();
}

std::string datetime::to_string() const
{
    if (_is_null)
    {
        return {};
    }
    else
    {
        return std::to_string(year) + "/" + std::to_string(month) + "/" + std::to_string(day) + " " + std::to_string(hour)
            + ":" + std::to_string(minute) + ":" + std::to_string(second) + ":" + std::to_string(microsecond);
    }
}

datetime datetime::now()
{
    return datetime(date::today(), time::now());
}

datetime datetime::today()
{
    return datetime(date::today(), time(0, 0, 0, 0));
}

datetime::datetime(int year_, int month_, int day_, int hour_, int minute_, int second_, int microsecond_)
    : year(year_), month(month_), day(day_), hour(hour_), minute(minute_), second(second_), microsecond(microsecond_), _is_null(false)
{
}

datetime::datetime(const date &d, const time &t)
    : hour(t.hour),
      minute(t.minute),
      second(t.second),
      microsecond(t.microsecond),
      _is_null(d.is_null())
{
    if (!d.is_null())
    {
        year = d.get_year();
        month = d.get_month();
        day = d.get_day();      
    }
}

int datetime::weekday() const
{
    return date(year, month, day).weekday();
}

int datetime::get_year() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty year of xlnt::datetime");
    }

    return year;
}

int datetime::get_month() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty month of xlnt::datetime");
    }

    return month;
}

int datetime::get_day() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty day of xlnt::datetime");
    }

    return day;
}

int datetime::get_hour() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty hour of xlnt::datetime");
    }

    return hour;
}

int datetime::get_minute() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty minute of xlnt::datetime");
    }

    return minute;
}

int datetime::get_second() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty second of xlnt::datetime");
    }

    return second;
}

int datetime::get_microsecond() const
{
    if (_is_null)
    {
        throw xlnt::invalid_attribute("access to invalid/empty microsecond of xlnt::datetime");
    }

    return microsecond;
}

datetime datetime::from_iso_string(const std::string &string)
{
    xlnt::datetime result(1900, 1, 1);

    auto separator_index = string.find('-');
    result.year = std::stoi(string.substr(0, separator_index));
    result.month = std::stoi(string.substr(separator_index + 1, string.find('-', separator_index + 1)));
    separator_index = string.find('-', separator_index + 1);
    result.day = std::stoi(string.substr(separator_index + 1, string.find('T', separator_index + 1)));
    separator_index = string.find('T', separator_index + 1);
    result.hour = std::stoi(string.substr(separator_index + 1, string.find(':', separator_index + 1)));
    separator_index = string.find(':', separator_index + 1);
    result.minute = std::stoi(string.substr(separator_index + 1, string.find(':', separator_index + 1)));
    separator_index = string.find(':', separator_index + 1);
    result.second = std::stoi(string.substr(separator_index + 1, string.find('Z', separator_index + 1)));

    return result;
}

std::string datetime::to_iso_string() const
{
    if (_is_null)
    {
        return {};
    }
    else
    {
        return std::to_string(year) + "-" + fill(std::to_string(month)) + "-" + fill(std::to_string(day)) + "T"
            + fill(std::to_string(hour)) + ":" + fill(std::to_string(minute)) + ":" + fill(std::to_string(second)) + "Z";
    }
}

} // namespace xlnt
