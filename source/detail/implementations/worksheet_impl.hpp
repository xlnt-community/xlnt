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
#include <unordered_map>
#include <vector>

#include <xlnt/drawing/spreadsheet_drawing.hpp>
#include <xlnt/packaging/ext_list.hpp>
#include <xlnt/workbook/named_range.hpp>
#include <xlnt/worksheet/column_properties.hpp>
#include <xlnt/worksheet/header_footer.hpp>
#include <xlnt/worksheet/phonetic_pr.hpp>
#include <xlnt/worksheet/range.hpp>
#include <xlnt/worksheet/range_reference.hpp>
#include <xlnt/worksheet/row_properties.hpp>
#include <xlnt/worksheet/sheet_format_properties.hpp>
#include <xlnt/worksheet/sheet_view.hpp>
#include <xlnt/worksheet/print_options.hpp>
#include <xlnt/worksheet/sheet_pr.hpp>
#include <detail/implementations/cell_impl.hpp>
#include <detail/implementations/workbook_impl.hpp>

namespace xlnt {

class workbook;

namespace detail {

struct worksheet_impl
{
private:
    worksheet_impl() = default;

public:
    worksheet_impl(workbook *parent_workbook, std::size_t id, const std::string &title)
        : parent_(parent_workbook->d_),
          id_(id),
          title_(title)
    {
    }

    ~worksheet_impl() = default;
    worksheet_impl(worksheet_impl &&other) noexcept = default;
    worksheet_impl &operator=(worksheet_impl &&other) noexcept = default;


    // Deleted copy constructor and assignment operator, as this class should be used with shared_ptr.
    worksheet_impl(const worksheet_impl &other) = delete;
    worksheet_impl &operator=(const worksheet_impl &other) = delete;

    std::shared_ptr<worksheet_impl> clone()
    {
        // Note: make_shared cannot be easily used with private constructors.
        auto clone = std::shared_ptr<worksheet_impl>(new worksheet_impl());
        clone->parent_ = parent_;

        clone->cell_map_.reserve(cell_map_.size());
        for (const auto &cell_pair : cell_map_)
        {
            auto it_inserted = clone->cell_map_.emplace(cell_pair.first, std::make_shared<detail::cell_impl>(*cell_pair.second));
            it_inserted.first->second->parent_ = clone;
        }

        clone->id_ = id_;
        clone->title_ = title_;
        clone->format_properties_ = format_properties_;
        clone->column_properties_ = column_properties_;
        clone->row_properties_ = row_properties_;
        clone->page_setup_ = page_setup_;
        clone->auto_filter_ = auto_filter_;
        clone->page_margins_ = page_margins_;
        clone->merged_cells_ = merged_cells_;
        clone->named_ranges_ = named_ranges_;
        clone->phonetic_properties_ = phonetic_properties_;
        clone->header_footer_ = header_footer_;
        clone->print_title_cols_ = print_title_cols_;
        clone->print_title_rows_ = print_title_rows_;
        clone->print_area_ = print_area_;
        clone->views_ = views_;
        clone->column_breaks_ = column_breaks_;
        clone->row_breaks_ = row_breaks_;
        clone->extension_list_ = extension_list_;
        clone->sheet_properties_ = sheet_properties_;
        clone->print_options_ = print_options_;

        return clone;
    }

    std::weak_ptr<workbook_impl> parent_;

    bool operator==(const worksheet_impl& rhs) const
    {
        // not comparing parent, id, title (title must be unique)
        if (cell_map_.size() != rhs.cell_map_.size())
        {
            return false;
        }
        else
        {
            for (const auto &cell_pair : cell_map_)
            {
                auto it = rhs.cell_map_.find(cell_pair.first);
                if (it == rhs.cell_map_.end())
                {
                    return false;
                }
                else if (*cell_pair.second != *it->second)
                {
                    return false;
                }
            }
        }

        return format_properties_ == rhs.format_properties_
            && column_properties_ == rhs.column_properties_
            && row_properties_ == rhs.row_properties_
            && page_setup_ == rhs.page_setup_
            && auto_filter_ == rhs.auto_filter_
            && page_margins_ == rhs.page_margins_
            && merged_cells_ == rhs.merged_cells_
            && named_ranges_ == rhs.named_ranges_
            && phonetic_properties_ == rhs.phonetic_properties_
            && header_footer_ == rhs.header_footer_
            && print_title_cols_ == rhs.print_title_cols_
            && print_title_rows_ == rhs.print_title_rows_
            && print_area_ == rhs.print_area_
            && views_ == rhs.views_
            && column_breaks_ == rhs.column_breaks_
            && row_breaks_ == rhs.row_breaks_
            && print_options_ == rhs.print_options_
            && sheet_properties_ == rhs.sheet_properties_
            && extension_list_ == rhs.extension_list_;
    }

    bool operator!=(const worksheet_impl& rhs) const
    {
        return !(*this == rhs);
    }

    std::size_t id_ = 0;
    std::string title_;

    sheet_format_properties format_properties_;

    std::unordered_map<column_t, column_properties> column_properties_;
    std::unordered_map<row_t, row_properties> row_properties_;

    std::unordered_map<cell_reference, std::shared_ptr<cell_impl>> cell_map_;

    optional<page_setup> page_setup_;
    optional<range_reference> auto_filter_;
    optional<page_margins> page_margins_;
    std::vector<range_reference> merged_cells_;
    std::unordered_map<std::string, named_range> named_ranges_;

    optional<phonetic_pr> phonetic_properties_;
    optional<header_footer> header_footer_;

    optional<std::pair<column_t, column_t>> print_title_cols_;
    optional<std::pair<row_t, row_t>> print_title_rows_;
    optional<range_reference> print_area_;

    std::vector<sheet_view> views_;

    std::vector<column_t> column_breaks_;
    std::vector<row_t> row_breaks_;

    optional<print_options> print_options_;
    optional<sheet_pr> sheet_properties_;

    optional<ext_list> extension_list_;

    std::string drawing_rel_id_;
    optional<drawing::spreadsheet_drawing> drawing_;
};

} // namespace detail
} // namespace xlnt
