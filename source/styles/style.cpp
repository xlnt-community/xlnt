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

#include <xlnt/styles/alignment.hpp>
#include <xlnt/styles/border.hpp>
#include <xlnt/styles/fill.hpp>
#include <xlnt/styles/font.hpp>
#include <xlnt/styles/number_format.hpp>
#include <xlnt/styles/protection.hpp>
#include <xlnt/styles/style.hpp>
#include <detail/implementations/style_impl.hpp>
#include <detail/implementations/stylesheet.hpp>

namespace {

std::vector<xlnt::number_format>::iterator find_number_format(
    std::vector<xlnt::number_format> &number_formats, std::size_t id)
{
    return std::find_if(number_formats.begin(), number_formats.end(),
        [=](const xlnt::number_format &nf) { return nf.id() == id; });
}

} // namespace

namespace xlnt {

style::style(std::shared_ptr<detail::style_impl> d)
    : d_(std::move(d))
{
    if (d_ == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::style: invalid style_impl pointer");
    }
}

style style::clone(clone_method method) const
{
    switch (method)
    {
    case clone_method::deep_copy:
        return style(std::make_shared<detail::style_impl>(*d_));
    case clone_method::shallow_copy:
        return style(d_);
    default:
        throw xlnt::invalid_parameter("clone method not supported");
    }
}

bool style::hidden() const
{
    return d_->hidden_style;
}

style style::hidden(bool value)
{
    d_->hidden_style = value;
    return style(d_);
}

std::size_t style::builtin_id() const
{
    return d_->builtin_id.get();
}

bool style::builtin() const
{
    return d_->builtin_id.is_set();
}

std::string style::name() const
{
    return d_->name;
}

style style::name(const std::string &name)
{
    d_->name = name;
    return *this;
}

bool style::custom_builtin() const
{
    return d_->builtin_id.is_set() && d_->custom_builtin;
}

bool style::compare(const style &other, bool compare_by_reference) const
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

bool style::operator==(const style &other) const
{
    return compare(other, true);
}

bool style::operator!=(const style &other) const
{
    return !operator==(other);
}

xlnt::alignment style::alignment() const
{
    return get_parent_checked()->alignments.at(d_->alignment_id.get());
}

style style::alignment(const xlnt::alignment &new_alignment, optional<bool> applied)
{
    auto parent = get_parent_checked();
    d_->alignment_id = parent->find_or_add(parent->alignments, new_alignment);
    d_->alignment_applied = applied;

    return *this;
}

xlnt::border style::border() const
{
    return get_parent_checked()->borders.at(d_->border_id.get());
}

style style::border(const xlnt::border &new_border, optional<bool> applied)
{
    auto parent = get_parent_checked();
    d_->border_id = parent->find_or_add(parent->borders, new_border);
    d_->border_applied = applied;

    return *this;
}

xlnt::fill style::fill() const
{
    return get_parent_checked()->fills.at(d_->fill_id.get());
}

style style::fill(const xlnt::fill &new_fill, optional<bool> applied)
{
    auto parent = get_parent_checked();
    d_->fill_id = parent->find_or_add(parent->fills, new_fill);
    d_->fill_applied = applied;

    return *this;
}

xlnt::font style::font() const
{
    return get_parent_checked()->fonts.at(d_->font_id.get());
}

style style::font(const xlnt::font &new_font, optional<bool> applied)
{
    auto parent = get_parent_checked();
    d_->font_id = parent->find_or_add(parent->fonts, new_font);
    d_->font_applied = applied;

    return *this;
}

xlnt::number_format style::number_format() const
{
    auto parent = get_parent_checked();
    auto match = find_number_format(parent->number_formats,
        d_->number_format_id.get());

    if (match == parent->number_formats.end())
    {
        throw invalid_attribute();
    }

    return *match;
}

style style::number_format(const xlnt::number_format &new_number_format, optional<bool> applied)
{
    auto copy = new_number_format;
    auto parent = get_parent_checked();

    if (!copy.has_id())
    {
        copy.id(parent->next_custom_number_format_id());
        parent->number_formats.push_back(copy);
    }
    else if (find_number_format(parent->number_formats, copy.id())
        == parent->number_formats.end())
    {
        parent->number_formats.push_back(copy);
    }

    d_->number_format_id = copy.id();
    d_->number_format_applied = applied;

    return *this;
}

xlnt::protection style::protection() const
{
    return get_parent_checked()->protections.at(d_->protection_id.get());
}

style style::protection(const xlnt::protection &new_protection, optional<bool> applied)
{
    auto parent = get_parent_checked();
    d_->protection_id = parent->find_or_add(parent->protections, new_protection);
    d_->protection_applied = applied;

    return *this;
}

bool style::alignment_applied() const
{
    return d_->alignment_applied.is_set()
        ? d_->alignment_applied.get()
        : d_->alignment_id.is_set();
}

bool style::border_applied() const
{
    return d_->border_applied.is_set()
        ? d_->border_applied.get()
        : d_->border_id.is_set();
}

bool style::fill_applied() const
{
    return d_->fill_applied.is_set()
        ? d_->fill_applied.get()
        : d_->fill_id.is_set();
}

bool style::font_applied() const
{
    return d_->font_applied.is_set()
        ? d_->font_applied.get()
        : d_->font_id.is_set();
}

bool style::number_format_applied() const
{
    return d_->number_format_applied.is_set()
        ? d_->number_format_applied.get()
        : d_->number_format_id.is_set();
}

bool style::protection_applied() const
{
    return d_->protection_applied.is_set()
        ? d_->protection_applied.get()
        : d_->protection_id.is_set();
}

bool style::pivot_button() const
{
    return d_->pivot_button_;
}

void style::pivot_button(bool show)
{
    d_->pivot_button_ = show;
}

bool style::quote_prefix() const
{
    return d_->quote_prefix_;
}

void style::quote_prefix(bool quote)
{
    d_->quote_prefix_ = quote;
}

std::shared_ptr<detail::stylesheet> style::get_parent_checked() const
{
    auto ptr = d_->parent.lock();

    if (ptr == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::style: invalid stylesheet pointer");
    }

    return ptr;
}

} // namespace xlnt
