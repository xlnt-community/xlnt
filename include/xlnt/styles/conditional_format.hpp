// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2010-2015 openpyxl
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

#include <memory>
#include <string>

#include <xlnt/xlnt_config.hpp>
#include <xlnt/utils/optional.hpp>
#include <xlnt/types.hpp>

namespace xlnt {

class border;
class fill;
class font;

namespace detail {

struct conditional_format_impl;
struct stylesheet;
class xlsx_consumer;
class xlsx_producer;

} // namespace detail

class XLNT_API condition
{
public:
    static condition text_starts_with(const std::string &start);
    static condition text_ends_with(const std::string &end);
    static condition text_contains(const std::string &start);
    static condition text_does_not_contain(const std::string &start);

    bool operator==(const condition &rhs) const
    {
        return text_comparand_ == rhs.text_comparand_;
    }

    bool operator!=(const condition &rhs) const
    {
        return !(*this == rhs);
    }

private:
    friend class detail::xlsx_producer;

    enum class type
    {
        contains_text
    } type_;

    enum class condition_operator
    {
        starts_with,
        ends_with,
        contains,
        does_not_contain
    } operator_;

    std::string text_comparand_;
};

/// <summary>
/// Describes a conditional format that will be applied to all cells in the
/// associated range that satisfy the condition. This can only be constructed
/// using methods on worksheet or range.
/// </summary>
class XLNT_API conditional_format
{
public:
    /// <summary>
    /// The method for cloning conditional_formats.
    /// </summary>
    using clone_method = xlnt::clone_method;

    /// <summary>
    /// Delete zero-argument constructor
    /// </summary>
    conditional_format() = delete;

    /// <summary>
    /// Creates a clone of this conditional_format. A shallow copy will copy the conditional_format's internal pointers,
    /// while a deep copy will copy all the internal structures and create a full clone of the conditional_format.
    /// </summary>
    conditional_format clone(clone_method method) const;

    // Formatting (xf) components

    /// <summary>
    ///
    /// </summary>
    bool has_border() const;

    /// <summary>
    ///
    /// </summary>
    class border border() const;

    /// <summary>
    ///
    /// </summary>
    conditional_format border(const xlnt::border &new_border);

    /// <summary>
    ///
    /// </summary>
    bool has_fill() const;

    /// <summary>
    ///
    /// </summary>
    class fill fill() const;

    /// <summary>
    ///
    /// </summary>
    conditional_format fill(const xlnt::fill &new_fill);

    /// <summary>
    ///
    /// </summary>
    bool has_font() const;

    /// <summary>
    ///
    /// </summary>
    class font font() const;

    /// <summary>
    ///
    /// </summary>
    conditional_format font(const xlnt::font &new_font);

    /// <summary>
    /// Returns true if this conditional_format is equal to other. If compare_by_reference is true, the comparison
    /// will only check that both conditional_formats point to the same internal conditional_format. Otherwise,
    /// if compare_by_reference is false, all conditional_format properties are compared.
    /// </summary>
    bool compare(const conditional_format &other, bool compare_by_reference) const;

    /// <summary>
    /// Returns true if this format is equivalent to other.
    /// </summary>
    bool operator==(const conditional_format &other) const;

    /// <summary>
    /// Returns true if this format is not equivalent to other.
    /// </summary>
    bool operator!=(const conditional_format &other) const;

private:
    friend struct detail::stylesheet;
    friend class detail::xlsx_consumer;

    /// <summary>
    ///
    /// </summary>
    conditional_format(std::shared_ptr<detail::conditional_format_impl> d);

    /// <summary>
    ///
    /// </summary>
    std::shared_ptr<detail::conditional_format_impl> d_;

    /// <summary>
    /// A pointer to the parent, ensuring it lives as long as its child (this instance) lives.
    /// </summary>
    std::shared_ptr<detail::stylesheet> parent_;
};

} // namespace xlnt
