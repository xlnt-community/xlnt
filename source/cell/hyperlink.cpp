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

#include <xlnt/cell/hyperlink.hpp>
#include <xlnt/utils/exceptions.hpp>
#include <detail/implementations/hyperlink_impl.hpp>

namespace xlnt {

hyperlink::hyperlink(std::shared_ptr<detail::hyperlink_impl> d)
    : d_(std::move(d))
{
    if (d_ == nullptr)
    {
        throw xlnt::invalid_attribute("xlnt::hyperlink: invalid hyperlink_impl pointer");
    }
}

hyperlink hyperlink::clone(clone_method method) const
{
    switch (method)
    {
    case clone_method::deep_copy:
        return hyperlink(std::make_shared<detail::hyperlink_impl>(*d_));
    case clone_method::shallow_copy:
        return hyperlink(d_);
    default:
        throw xlnt::invalid_parameter("clone method not supported");
    }
}

bool hyperlink::compare(const hyperlink &other, bool compare_by_reference) const
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

bool hyperlink::operator==(const hyperlink &comparand) const
{
    return compare(comparand, true);
}

bool hyperlink::operator!=(const hyperlink &comparand) const
{
    return !(*this == comparand);
}

relationship hyperlink::relationship() const
{
    if (!external())
    {
        throw xlnt::exception("only external hyperlinks have associated relationships");
    }

    return d_->relationship;
}

std::string hyperlink::url() const
{
    if (!external())
    {
        throw xlnt::exception("only external hyperlinks have associated urls");
    }

    return d_->relationship.target().to_string();
}

std::string hyperlink::target_range() const
{
    if (external())
    {
        throw xlnt::exception("only internal hyperlinks have a target range");
    }

    return d_->relationship.target().to_string();
}

bool hyperlink::external() const
{
    return d_->relationship.target_mode() == target_mode::external;
}

bool hyperlink::has_display() const
{
    return d_->display.is_set();
}

void hyperlink::display(const std::string &value)
{
    d_->display.set(value);
}

const std::string &hyperlink::display() const
{
    return d_->display.get();
}

bool hyperlink::has_tooltip() const
{
    return d_->tooltip.is_set();
}

void hyperlink::tooltip(const std::string &value)
{
    d_->tooltip.set(value);
}

const std::string &hyperlink::tooltip() const
{
    return d_->tooltip.get();
}

bool hyperlink::has_location() const
{
    return d_->location.is_set();
}

void hyperlink::location(const std::string &value)
{
    d_->location.set(value);
}

const std::string &hyperlink::location() const
{
    return d_->location.get();
}

} // namespace xlnt
