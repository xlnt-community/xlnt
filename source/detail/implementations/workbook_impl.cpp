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

#include <detail/implementations/workbook_impl.hpp>
#include <detail/implementations/worksheet_impl.hpp>

namespace xlnt {
namespace detail {

workbook_impl::workbook_impl(const workbook_impl &other)
    : active_sheet_index_(other.active_sheet_index_),
        shared_strings_ids_(other.shared_strings_ids_),
        shared_strings_values_(other.shared_strings_values_),
        stylesheet_(other.stylesheet_ == nullptr ? nullptr : std::make_shared<stylesheet>(*other.stylesheet_)),
        manifest_(other.manifest_),
        theme_(other.theme_),
        core_properties_(other.core_properties_),
        extended_properties_(other.extended_properties_),
        custom_properties_(other.custom_properties_),
        view_(other.view_),
        code_name_(other.code_name_),
        file_version_(other.file_version_)
{
    worksheets_.reserve(other.worksheets_.size());
    for (const auto &worksheet_ptr : other.worksheets_)
    {
        worksheets_.emplace_back(worksheet_ptr->clone());
    }
}

workbook_impl &workbook_impl::operator=(const workbook_impl &other)
{
    active_sheet_index_ = other.active_sheet_index_;
    worksheets_.clear();
    worksheets_.reserve(other.worksheets_.size());
    for (const auto &worksheet_ptr : other.worksheets_)
    {
        worksheets_.emplace_back(worksheet_ptr->clone());
    }

    if (other.stylesheet_ == nullptr)
    {
        stylesheet_ = nullptr;
    }
    else if (stylesheet_ == nullptr)
    {
        stylesheet_ = std::make_shared<stylesheet>(*other.stylesheet_);
    }
    else
    {
        *stylesheet_ = *other.stylesheet_;
    }

    shared_strings_ids_ = other.shared_strings_ids_;
    shared_strings_values_ = other.shared_strings_values_;
    theme_ = other.theme_;
    manifest_ = other.manifest_;

    sheet_title_rel_id_map_ = other.sheet_title_rel_id_map_;
    sheet_hidden_ = other.sheet_hidden_;
    view_ = other.view_;
    code_name_ = other.code_name_;
    file_version_ = other.file_version_;

    core_properties_ = other.core_properties_;
    extended_properties_ = other.extended_properties_;
    custom_properties_ = other.custom_properties_;

    return *this;
}

bool workbook_impl::operator==(const workbook_impl &other) const
{
    // not comparing abs_path_
    if (worksheets_.size() != other.worksheets_.size())
    {
        return false;
    }
    else
    {
        auto it_this = worksheets_.begin();
        auto it_other = other.worksheets_.begin();
        for (; it_this != worksheets_.end() && it_other != other.worksheets_.end(); ++it_this, ++it_other)
        {
            if (**it_this != **it_other)
            {
                return false;
            }
        }
    }

    return active_sheet_index_ == other.active_sheet_index_
        && shared_strings_ids_ == other.shared_strings_ids_
        && ((stylesheet_ == nullptr && other.stylesheet_ == nullptr) || (stylesheet_ != nullptr && other.stylesheet_ != nullptr && *stylesheet_ == *other.stylesheet_))
        && base_date_ == other.base_date_
        && title_ == other.title_
        && manifest_ == other.manifest_
        && theme_ == other.theme_
        && images_ == other.images_
        && binaries_ == other.binaries_
        && core_properties_ == other.core_properties_
        && extended_properties_ == other.extended_properties_
        && custom_properties_ == other.custom_properties_
        && sheet_title_rel_id_map_ == other.sheet_title_rel_id_map_
        && sheet_hidden_ == other.sheet_hidden_
        && view_ == other.view_
        && code_name_ == other.code_name_
        && file_version_ == other.file_version_
        && calculation_properties_ == other.calculation_properties_
        && arch_id_flags_ == other.arch_id_flags_
        && extensions_ == other.extensions_;
}

} // namespace detail
} // namespace xlnt
