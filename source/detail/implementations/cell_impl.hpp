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
#pragma once

#include <memory>
#include <string>

#include <xlnt/cell/cell_type.hpp>
#include <xlnt/cell/comment.hpp>
#include <xlnt/cell/index_types.hpp>
#include <xlnt/cell/rich_text.hpp>
#include <xlnt/packaging/relationship.hpp>
#include <xlnt/utils/optional.hpp>
#include <detail/implementations/format_impl.hpp>
#include <detail/implementations/hyperlink_impl.hpp>

namespace xlnt {
namespace detail {

struct worksheet_impl;

struct cell_impl
{
    cell_impl() = default;
    ~cell_impl() = default;
    cell_impl(cell_impl &&other) noexcept = default;
    cell_impl &operator=(cell_impl &&other) noexcept = default;

    cell_impl(const cell_impl &other)
    {
        *this = other;
    }

    cell_impl &operator=(const cell_impl &other)
    {
        type_ = other.type_;
        parent_ = other.parent_;
        column_ = other.column_;
        row_ = other.row_;
        is_merged_ = other.is_merged_;
        phonetics_visible_ = other.phonetics_visible_;
        value_text_ = other.value_text_;
        value_numeric_ = other.value_numeric_;
        formula_ = other.formula_;
        format_ = other.format_;
        comment_ = other.comment_;

        if (!other.hyperlink_.is_set())
        {
            hyperlink_.clear();
        }
        else if (!hyperlink_.is_set())
        {
            hyperlink_.set(std::make_shared<hyperlink_impl>(*other.hyperlink_.get()));
        }
        else
        {
            *hyperlink_.get() = *other.hyperlink_.get();
        }

        return *this;
    }

    cell_type type_ = cell_type::empty;

    std::weak_ptr<worksheet_impl> parent_;

    column_t column_ = 1;
    row_t row_ = 1;

    bool is_merged_ = false;
    bool phonetics_visible_ = false;

    rich_text value_text_;
    double value_numeric_ = 0.0;

    optional<std::string> formula_;
    optional<std::shared_ptr<hyperlink_impl>> hyperlink_;
    optional<std::shared_ptr<format_impl>> format_;
    optional<std::shared_ptr<comment>> comment_;

    bool is_garbage_collectible() const
    {
        return !(type_ != cell_type::empty || is_merged_ || phonetics_visible_ || formula_.is_set() || format_.is_set() || hyperlink_.is_set());
    }
};

inline bool operator==(const cell_impl &lhs, const cell_impl &rhs)
{
    // not comparing parent, row, column
    return lhs.type_ == rhs.type_
        && lhs.is_merged_ == rhs.is_merged_
        && lhs.phonetics_visible_ == rhs.phonetics_visible_
        && lhs.value_text_ == rhs.value_text_
        && float_equals(lhs.value_numeric_, rhs.value_numeric_)
        && lhs.formula_ == rhs.formula_
        && (lhs.hyperlink_.is_set() == rhs.hyperlink_.is_set() && (!lhs.hyperlink_.is_set() || *lhs.hyperlink_.get() == *rhs.hyperlink_.get()))
        && (lhs.format_.is_set() == rhs.format_.is_set() && (!lhs.format_.is_set() || *lhs.format_.get() == *rhs.format_.get()))
        && (lhs.comment_.is_set() == rhs.comment_.is_set() && (!lhs.comment_.is_set() || *lhs.comment_.get() == *rhs.comment_.get()));
}

inline bool operator!=(const cell_impl &lhs, const cell_impl &rhs)
{
    return !(lhs == rhs);
}

} // namespace detail
} // namespace xlnt
