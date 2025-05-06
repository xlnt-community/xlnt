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

#include <xlnt/styles/border.hpp>
#include <xlnt/styles/conditional_format.hpp>
#include <xlnt/styles/fill.hpp>
#include <xlnt/styles/font.hpp>
#include <detail/implementations/conditional_format_impl.hpp>
#include <detail/implementations/stylesheet.hpp>

namespace xlnt {

condition condition::text_starts_with(const std::string &text)
{
    condition c;
    c.type_ = type::contains_text;
    c.operator_ = condition_operator::starts_with;
    c.text_comparand_ = text;
    return c;
}

condition condition::text_ends_with(const std::string &text)
{
    condition c;
    c.type_ = type::contains_text;
    c.operator_ = condition_operator::ends_with;
    c.text_comparand_ = text;
    return c;
}

condition condition::text_contains(const std::string &text)
{
    condition c;
    c.type_ = type::contains_text;
    c.operator_ = condition_operator::contains;
    c.text_comparand_ = text;
    return c;
}

condition condition::text_does_not_contain(const std::string &text)
{
    condition c;
    c.type_ = type::contains_text;
    c.operator_ = condition_operator::does_not_contain;
    c.text_comparand_ = text;
    return c;
}

conditional_format::conditional_format(std::shared_ptr<detail::conditional_format_impl> d)
    : d_(std::move(d))
{
    if (d_ == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::conditional_format: invalid conditional_format_impl pointer");
    }

    parent_ = d_->parent.lock();

    if (parent_ == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::conditional_format: invalid stylesheet pointer");
    }
}

conditional_format conditional_format::clone(clone_method method) const
{
    switch (method)
    {
    case clone_method::deep_copy:
        return conditional_format(std::make_shared<detail::conditional_format_impl>(*d_));
    case clone_method::shallow_copy:
        return conditional_format(d_);
    default:
        throw xlnt::invalid_parameter("clone method not supported");
    }
}

bool conditional_format::compare(const conditional_format &other, bool compare_by_reference) const
{
    if (compare_by_reference)
    {
        return d_ == other.d_;
    }
    else
    {
        return *d_ == *other.d_;
    }
}

bool conditional_format::operator==(const conditional_format &other) const
{
    return compare(other, true);
}

bool conditional_format::operator!=(const conditional_format &other) const
{
    return !(*this == other);
}

bool conditional_format::has_border() const
{
    return d_->border_id.is_set();
}

xlnt::border conditional_format::border() const
{
    return parent_->borders.at(d_->border_id.get());
}

conditional_format conditional_format::border(const xlnt::border &new_border)
{
    d_->border_id = parent_->find_or_add(parent_->borders, new_border);
    return *this;
}

bool conditional_format::has_fill() const
{
    return d_->fill_id.is_set();
}

xlnt::fill conditional_format::fill() const
{
    return parent_->fills.at(d_->fill_id.get());
}

conditional_format conditional_format::fill(const xlnt::fill &new_fill)
{
    d_->fill_id = parent_->find_or_add(parent_->fills, new_fill);
    return *this;
}

bool conditional_format::has_font() const
{
    return d_->font_id.is_set();
}

xlnt::font conditional_format::font() const
{
    return parent_->fonts.at(d_->font_id.get());
}

conditional_format conditional_format::font(const xlnt::font &new_font)
{
    d_->font_id = parent_->find_or_add(parent_->fonts, new_font);
    return *this;
}

} // namespace xlnt
