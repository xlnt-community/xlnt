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

// detail imports must come first in this file.
#include <detail/implementations/format_impl.hpp>
#include <detail/implementations/stylesheet.hpp>

#include <xlnt/styles/format.hpp>
#include <xlnt/styles/style.hpp>

namespace xlnt {

format::format(std::shared_ptr<detail::format_impl> d)
    : d_(std::move(d))
{
    if (d_ == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::format: invalid format_impl pointer");
    }
}

format format::clone(clone_method method) const
{
    switch (method)
    {
    case clone_method::deep_copy:
    {
        format format(std::make_shared<detail::format_impl>(*d_));
        /*format.d_->references = 0;

        optional<std::size_t> alignment_id;
        optional<std::size_t> border_id;
        optional<std::size_t> fill_id;
        optional<std::size_t> font_id;
        optional<std::size_t> number_format_id;
        optional<std::size_t> protection_id;

        if (format.d_->alignment_id.is_set())
        {
            xlnt::alignment alignment = format.alignment();
            alignment.;
        }

        if (format.d_->border_id.is_set())
        {
            format.border();
        }

        if (format.d_->fill_id.is_set())
        {
            format.fill();
        }

        if (format.d_->font_id.is_set())
        {
            format.font();
        }

        if (format.d_->number_format_id.is_set())
        {
            xlnt::number_format number_format = format.number_format();
            number_format.id(std::numeric_limits<decltype(format.d_->number_format_id)::value_type>::max());
            format.number_format(number_format, format.d_->number_format_applied);
        }

        if (format.d_->protection_id.is_set())
        {
            format.protection();
            }*/

        return format;
    }
    case clone_method::shallow_copy:
        return format(d_);
    default:
        throw xlnt::invalid_parameter("clone method not supported");
    }
}

bool format::compare(const format &other, bool compare_by_reference) const
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

bool format::operator==(const format &other) const
{
    return compare(other, true);
}

bool format::operator!=(const format &other) const
{
    return !(*this == other);
}

void format::clear_style()
{
    d_->style.clear();
}

format format::style(const xlnt::style &new_style)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_style.name());
    return format(d_);
}

format format::style(const std::string &new_style)
{
    d_->style = new_style;
    return format(d_);
}

bool format::has_style() const
{
    return d_->style.is_set();
}

style format::style()
{
    if (!has_style())
    {
        throw invalid_attribute();
    }

    return get_parent_checked()->style(d_->style.get());
}

const style format::style() const
{
    if (!has_style())
    {
        throw invalid_attribute();
    }

    return get_parent_checked()->style(d_->style.get());
}

xlnt::alignment format::alignment() const
{
    return get_parent_checked()->alignments.at(d_->alignment_id.get());
}

format format::alignment(const xlnt::alignment &new_alignment, optional<bool> applied)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_alignment, applied);
    return format(d_);
}

xlnt::border format::border() const
{
    return get_parent_checked()->borders.at(d_->border_id.get());
}

format format::border(const xlnt::border &new_border, optional<bool> applied)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_border, applied);
    return format(d_);
}

xlnt::fill format::fill() const
{
    return get_parent_checked()->fills.at(d_->fill_id.get());
}

format format::fill(const xlnt::fill &new_fill, optional<bool> applied)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_fill, applied);
    return format(d_);
}

xlnt::font format::font() const
{
    return get_parent_checked()->fonts.at(d_->font_id.get());
}

format format::font(const xlnt::font &new_font, optional<bool> applied)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_font, applied);
    return format(d_);
}

xlnt::number_format format::number_format() const
{
    if (d_->number_format_id.is_set())
    {
        const auto number_format_id = d_->number_format_id.get();

        if (number_format::is_builtin_format(number_format_id))
        {
            return number_format::from_builtin_id(number_format_id);
        }

        auto parent = get_parent_checked();
        const auto it = std::find_if(parent->number_formats.begin(),
                                     parent->number_formats.end(),
                                     [number_format_id](const xlnt::number_format &nf)
                                     {
                                         return nf.id() == number_format_id;
                                     });
        if (it != parent->number_formats.end())
        {
            return *it;
        }
    }

    return xlnt::number_format();
}

format format::number_format(const xlnt::number_format &new_number_format, optional<bool> applied)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_number_format, applied);
    return format(d_);
}

xlnt::protection format::protection() const
{
    return get_parent_checked()->protections.at(d_->protection_id.get());
}

format format::protection(const xlnt::protection &new_protection, optional<bool> applied)
{
    d_ = get_parent_checked()->find_or_create_with(d_.get(), new_protection, applied);
    return format(d_);
}

bool format::alignment_applied() const
{
    return d_->alignment_applied.is_set()
        ? d_->alignment_applied.get()
        : d_->alignment_id.is_set();
}

bool format::border_applied() const
{
    return d_->border_applied.is_set()
        ? d_->border_applied.get()
        : d_->border_id.is_set();
}

bool format::fill_applied() const
{
    return d_->fill_applied.is_set()
        ? d_->fill_applied.get()
        : d_->fill_id.is_set();
}

bool format::font_applied() const
{
    return d_->font_applied.is_set()
        ? d_->font_applied.get()
        : d_->font_id.is_set();
}

bool format::number_format_applied() const
{
    return d_->number_format_applied.is_set()
        ? d_->number_format_applied.get()
        : d_->number_format_id.is_set();
}

bool format::protection_applied() const
{
    return d_->protection_applied.is_set()
        ? d_->protection_applied.get()
        : d_->protection_id.is_set();
}

bool format::pivot_button() const
{
    return d_->pivot_button_;
}

void format::pivot_button(bool show)
{
    d_->pivot_button_ = show;
}

bool format::quote_prefix() const
{
    return d_->quote_prefix_;
}

void format::quote_prefix(bool quote)
{
    d_->quote_prefix_ = quote;
}

std::shared_ptr<detail::stylesheet> format::get_parent_checked() const
{
    auto ptr = d_->parent.lock();

    if (ptr == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::format: invalid stylesheet pointer");
    }

    return ptr;
}

} // namespace xlnt
