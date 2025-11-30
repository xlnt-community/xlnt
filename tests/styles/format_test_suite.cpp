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

#include <xlnt/cell/cell.hpp>
#include <xlnt/styles/alignment.hpp>
#include <xlnt/styles/border.hpp>
#include <xlnt/styles/fill.hpp>
#include <xlnt/styles/font.hpp>
#include <xlnt/styles/format.hpp>
#include <xlnt/styles/number_format.hpp>
#include <xlnt/styles/protection.hpp>
#include <xlnt/styles/style.hpp>
#include <xlnt/utils/optional.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/worksheet.hpp>

class format_test_suite : public test_suite
{
public:
    format_test_suite()
    {
        register_test(test_alignment);
        register_test(test_border);
        register_test(test_fill);
        register_test(test_font);
        register_test(test_number_format);
        register_test(test_protection);
        register_test(test_style);
        register_test(test_issue93);
        register_test(test_format_garbage_collection);
    }

    void test_alignment()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_alignment());
        xlnt_assert_throws_nothing(format.alignment());

        xlnt::alignment alignment;
        format.alignment(alignment);

        xlnt_assert(format.has_alignment());
        xlnt_assert_equals(format.alignment(), alignment);
    }

    void test_border()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_border());
        xlnt_assert_throws_nothing(format.border());

        xlnt::border border;
        format.border(border);

        xlnt_assert(format.has_border());
        xlnt_assert_equals(format.border(), border);
    }

    void test_fill()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_fill());
        xlnt_assert_throws_nothing(format.fill());

        xlnt::fill fill;
        format.fill(fill);

        xlnt_assert(format.has_fill());
        xlnt_assert_equals(format.fill(), fill);
    }

    void test_font()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_font());
        xlnt_assert_throws_nothing(format.font());

        xlnt::font font;
        format.font(font);

        xlnt_assert(format.has_font());
        xlnt_assert_equals(format.font(), font);
    }

    void test_number_format()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_number_format());
        xlnt_assert_throws_nothing(format.number_format());

        xlnt::number_format number_format;
        format.number_format(number_format);

        xlnt_assert(format.has_number_format());
        xlnt_assert_equals(format.number_format(), number_format);
    }

    void test_protection()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_protection());
        xlnt_assert_throws_nothing(format.protection());

        xlnt::protection protection;
        format.protection(protection);

        xlnt_assert(format.has_protection());
        xlnt_assert_equals(format.protection(), protection);
    }

    void test_style()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format();
        xlnt_assert(!format.has_style());
        xlnt_assert_throws(format.style(), xlnt::invalid_attribute);

        xlnt::style style = wb.create_style("test_style");
        format.style(style);

        xlnt_assert(format.has_style());
        xlnt_assert_throws_nothing(format.style());
        format.clear_style();
        xlnt_assert(!format.has_style());
        xlnt_assert_throws(format.style(), xlnt::invalid_attribute);
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(format.clear_style());
    }

    void test_issue93()
    {
        {
            xlnt::workbook wb;
            xlnt::worksheet ws = wb.active_sheet();

            ws.cell("A1").fill(xlnt::fill(xlnt::gradient_fill()));
            xlnt_assert(ws.cell("A1").format().fill_applied());

            ws.cell("A2").fill(xlnt::fill(xlnt::gradient_fill()));
            xlnt_assert(ws.cell("A2").format().fill_applied());

            wb.save("temp.xlsx");
        }

        xlnt::workbook wb(xlnt::path("temp.xlsx"));
        xlnt::worksheet ws = wb.active_sheet();

        ws.cell("A1").fill(xlnt::fill(xlnt::pattern_fill()));

        xlnt_assert(ws.cell("A2").has_format());
        xlnt_assert(ws.cell("A2").format().fill_applied());
        xlnt_assert_equals(ws.cell("A2").format().fill(), xlnt::gradient_fill());

        ws.delete_rows(1, 1);

        xlnt_assert(ws.cell("A1").has_format());
        xlnt_assert(ws.cell("A1").format().fill_applied());
        xlnt_assert_equals(ws.cell("A1").format().fill(), xlnt::gradient_fill());
    }

    void test_format_garbage_collection()
    {
        xlnt::workbook wb;

        xlnt_assert_equals(wb.format_count(), 1); // workbook has a default format

        xlnt::worksheet ws = wb.active_sheet();

        ws.cell("A1").fill(xlnt::fill(xlnt::pattern_fill()));
        xlnt_assert_equals(wb.format_count(), 2);

        ws.cell("A2").fill(xlnt::fill(xlnt::pattern_fill()));
        xlnt_assert_equals(wb.format_count(), 2); // same format as A1

        ws.cell("A3").fill(xlnt::fill(xlnt::pattern_fill()));
        xlnt_assert_equals(wb.format_count(), 2); // same format as A1 and A2

        ws.cell("A1").fill(xlnt::fill(xlnt::gradient_fill()));
        xlnt_assert_equals(wb.format_count(), 3); // a new format

        ws.cell("A2").fill(xlnt::fill(xlnt::gradient_fill()));
        xlnt_assert_equals(wb.format_count(), 3); // same format as A1

        ws.cell("A3").fill(xlnt::fill(xlnt::gradient_fill()));
        xlnt_assert_equals(wb.format_count(), 2); // xlnt::pattern_fill format isn't used anymore, hence garbage collected

        ws.delete_rows(1, 2);
        xlnt_assert_equals(wb.format_count(), 2); // xlnt::gradient_fill is still used by A1 (i.e. the original A3 cell)
        ws.delete_rows(1, 1);
        xlnt_assert_equals(wb.format_count(), 1); // xlnt::gradient_fill format isn't used anymore, hence garbage collected

        ws.cell("A1").fill(xlnt::fill(xlnt::pattern_fill()));
        xlnt_assert_equals(wb.format_count(), 2);

        {
            xlnt::format f = ws.cell("A1").format();
            xlnt_assert_equals(wb.format_count(), 2);

            ws.delete_rows(1, 1);
            xlnt_assert_equals(wb.format_count(), 2); // format is still referenced by local variable f.
        }
        xlnt_assert_equals(wb.format_count(), 1);
    }
};
static format_test_suite x;
