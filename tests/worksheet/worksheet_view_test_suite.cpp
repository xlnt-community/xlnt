// Copyright (c) 2025 xlnt-community
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

#include <helpers/test_suite.hpp>

#include <xlnt/worksheet/worksheet.hpp>
#include <xlnt/workbook/workbook.hpp>

class worksheet_view_test_suite : public test_suite
{
public:
    worksheet_view_test_suite()
    {
        register_test(test_pane);
        register_test(test_top_left_cell);
    }

    void test_pane()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        xlnt::sheet_view& view = ws.view();
        xlnt_assert(!view.has_pane());
        xlnt_assert_throws(view.pane(), xlnt::invalid_attribute);

        xlnt::pane pane;
        view.pane(pane);

        xlnt_assert(view.has_pane());
        xlnt_assert_equals(view.pane(), pane);
    }

    void test_top_left_cell()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        xlnt::sheet_view& view = ws.view();
        xlnt_assert(!view.has_top_left_cell());
        xlnt_assert_throws(view.top_left_cell(), xlnt::invalid_attribute);

        xlnt::cell_reference cell_ref(1, 1);
        view.top_left_cell(cell_ref);

        xlnt_assert(view.has_top_left_cell());
        xlnt_assert_equals(view.top_left_cell(), cell_ref);
    }
};
static worksheet_view_test_suite x;
