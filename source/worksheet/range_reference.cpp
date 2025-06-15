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

#include <xlnt/worksheet/range_reference.hpp>

#include <detail/constants.hpp>
#include <xlnt/utils/exceptions.hpp>

namespace {

bool is_column_only(const std::string &s)
{
    if (s.empty())
    {
        return false;
    }

    size_t start_pos = (s[0] == '$') ? 1 : 0;
    if (start_pos >= s.length()) // String contains only a "$"
    {
        return false;
    }

    for (size_t i = start_pos; i < s.length(); ++i)
    {
        if (!std::isalpha(static_cast<unsigned char>(s[i])))
        {
            return false;
        }
    }

    return true;
}

bool is_row_only(const std::string &s)
{
    if (s.empty())
    {
        return false;
    }

    size_t start_pos = (s[0] == '$') ? 1 : 0;
    if (start_pos >= s.length()) // String contains only a "$"
    {
        return false;
    }

    for (size_t i = start_pos; i < s.length(); ++i)
    {
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
        {
            return false;
        }
    }

    return true;
}
}

namespace xlnt {

range_reference range_reference::make_absolute(const xlnt::range_reference &relative)
{
    range_reference copy = relative;

    copy.top_left_.make_absolute(true, true);
    copy.bottom_right_.make_absolute(true, true);

    return copy;
}

range_reference::range_reference()
    : range_reference("A1")
{
}

range_reference::range_reference(const char *range_string)
    : range_reference(std::string(range_string))
{
}

range_reference::range_reference(const std::string &range_string)
{
    // Handle special Excel error references like #REF!
    if (range_string == "#REF!")
    {
        // Create a default invalid range for #REF! references
        // This allows the workbook to load without throwing an exception
        top_left_ = cell_reference("A1");
        bottom_right_ = cell_reference("A1");
        return;
    }

    auto colon_index = range_string.find(':');

    if (colon_index == std::string::npos)
    {
        // Single cell reference, e.g., "A1"
        top_left_ = cell_reference(range_string);
        bottom_right_ = top_left_;
        return;
    }

    std::string start_part = range_string.substr(0, colon_index);
    std::string end_part = range_string.substr(colon_index + 1);

    // Handle cases where one or both parts are #REF!
    if (start_part == "#REF!" || end_part == "#REF!")
    {
        // Create a default invalid range for #REF! references
        top_left_ = cell_reference("A1");
        bottom_right_ = cell_reference("A1");
        return;
    }

    if (is_column_only(start_part) && is_column_only(end_part))
    {
        // Whole column reference, e.g., "A:C"
        if (start_part[0] == '$') start_part.erase(0, 1);
        if (end_part[0] == '$') end_part.erase(0, 1);

        top_left_ = cell_reference(start_part, 1);
        bottom_right_ = cell_reference(end_part, constants::max_row());
    }
    else if (is_row_only(start_part) && is_row_only(end_part))
    {
        // Whole row reference, e.g., "1:5"
        if (start_part[0] == '$') start_part.erase(0, 1);
        if (end_part[0] == '$') end_part.erase(0, 1);

        try
        {
            row_t start_row = static_cast<row_t>(std::stoul(start_part));
            row_t end_row = static_cast<row_t>(std::stoul(end_part));

            top_left_ = cell_reference(constants::min_column(), start_row);
            bottom_right_ = cell_reference(constants::max_column(), end_row);
        }
        catch (const std::out_of_range &)
        {
            std::string error_message = "Row number in range string is out of valid range: " + range_string;
            throw invalid_parameter(error_message.c_str());
        }
        catch (const std::invalid_argument &)
        {
            std::string error_message = "Invalid row number format in range string: " + range_string;
            throw invalid_parameter(error_message.c_str());
        }
    }
    else
    {
        top_left_ = cell_reference(start_part);
        bottom_right_ = cell_reference(end_part);
    }
}

range_reference::range_reference(const cell_reference &top_left,
    const cell_reference &bottom_right)
    : top_left_(top_left),
      bottom_right_(bottom_right)
{
}

range_reference::range_reference(column_t column_index_start,
    row_t row_index_start,
    column_t column_index_end,
    row_t row_index_end)
    : top_left_(column_index_start, row_index_start),
      bottom_right_(column_index_end, row_index_end)
{
}

range_reference range_reference::make_offset(int column_offset, int row_offset) const
{
    auto top_left = top_left_.make_offset(column_offset, row_offset);
    auto bottom_right = bottom_right_.make_offset(column_offset, row_offset);

    return range_reference(top_left, bottom_right);
}

std::size_t range_reference::height() const
{
    return 1 + bottom_right_.row() - top_left_.row();
}

std::size_t range_reference::width() const
{
    return 1 + (bottom_right_.column() - top_left_.column()).index;
}

bool range_reference::is_single_cell() const
{
    return width() == 1 && height() == 1;
}

std::string range_reference::to_string() const
{
    if (is_single_cell())
        return top_left().to_string();
    else
        return top_left_.to_string() + ":" + bottom_right_.to_string();
}

bool range_reference::operator==(const range_reference &comparand) const
{
    return comparand.top_left_ == top_left_ && comparand.bottom_right_ == bottom_right_;
}

bool range_reference::operator!=(const range_reference &comparand) const
{
    return !(*this == comparand);
}

cell_reference range_reference::top_left() const
{
    return top_left_;
}

cell_reference range_reference::top_right() const
{
    return cell_reference(bottom_right_.column(), top_left_.row());
}

cell_reference range_reference::bottom_left() const
{
    return cell_reference(top_left_.column(), bottom_right_.row());
}

cell_reference range_reference::bottom_right() const
{
    return bottom_right_;
}

bool range_reference::contains(const cell_reference &ref) const
{
    return top_left_.column_index() <= ref.column_index()
        && bottom_right_.column_index() >= ref.column_index()
        && top_left_.row() <= ref.row()
        && bottom_right_.row() >= ref.row();
}

bool range_reference::operator==(const std::string &reference_string) const
{
    return *this == range_reference(reference_string);
}

bool range_reference::operator==(const char *reference_string) const
{
    return *this == std::string(reference_string);
}

bool range_reference::operator!=(const std::string &reference_string) const
{
    return *this != range_reference(reference_string);
}

bool range_reference::operator!=(const char *reference_string) const
{
    return *this != std::string(reference_string);
}

bool operator==(const std::string &reference_string, const range_reference &ref)
{
    return ref == reference_string;
}

bool operator==(const char *reference_string, const range_reference &ref)
{
    return ref == reference_string;
}

bool operator!=(const std::string &reference_string, const range_reference &ref)
{
    return ref != reference_string;
}

bool operator!=(const char *reference_string, const range_reference &ref)
{
    return ref != reference_string;
}

} // namespace xlnt
