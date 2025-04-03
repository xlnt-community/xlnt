// Copyright (c) 2018-2022 Thomas Fussell
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

#include <xlnt/xlnt_config.hpp>
#include <xlnt/types.hpp>
#include <memory>
#include <string>

namespace xlnt {

namespace detail {
struct hyperlink_impl;
}

class cell;
class range;
class relationship;

/// <summary>
/// Describes a hyperlink pointing from a cell to another cell or a URL.
/// </summary>
class XLNT_API hyperlink
{
public:
    /// <summary>
    /// The method for cloning hyperlinks.
    /// </summary>
    using clone_method = xlnt::clone_method;

    /// <summary>
    /// Creates a clone of this hyperlink. A shallow copy will copy the hyperlink's internal pointers,
    /// while a deep copy will copy all the internal structures and create a full clone of the hyperlink.
    /// </summary>
    hyperlink clone(clone_method method) const;

    /// <summary>
    /// Returns true if this hyperlink is equal to other. If compare_by_reference is true, the comparison
    /// will only check that both hyperlinks point to the same internal hyperlink. Otherwise,
    /// if compare_by_reference is false, all hyperlink properties are compared.
    /// </summary>
    bool compare(const hyperlink &other, bool compare_by_reference) const;

    /// <summary>
    /// Returns true if this hyperlink is the same hyperlink as comparand (compared by reference).
    /// </summary>
    bool operator==(const hyperlink &comparand) const;

    /// <summary>
    /// Returns false if this hyperlink is the same hyperlink as comparand (compared by reference).
    /// </summary>
    bool operator!=(const hyperlink &comparand) const;

    bool external() const;
    class relationship relationship() const;
    // external target
    std::string url() const;
    // internal target
    std::string target_range() const;

    bool has_display() const;
    void display(const std::string &value);
    const std::string &display() const;

    bool has_tooltip() const;
    void tooltip(const std::string &value);
    const std::string &tooltip() const;

    bool has_location() const;
    void location(const std::string &value);
    const std::string &location() const;

private:
    friend class cell;
    hyperlink(std::shared_ptr<detail::hyperlink_impl> d);
    std::shared_ptr<detail::hyperlink_impl> d_;
};

} // namespace xlnt
