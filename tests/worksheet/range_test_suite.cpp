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

#include <helpers/test_suite.hpp>
#include <xlnt/cell/cell.hpp>
#include <xlnt/styles/font.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/header_footer.hpp>
#include <xlnt/worksheet/range.hpp>
#include <xlnt/worksheet/worksheet.hpp>
#include <detail/constants.hpp>

class range_test_suite : public test_suite
{
public:
    range_test_suite()
    {
        register_test(test_construction);
        register_test(test_batch_formatting);
        register_test(test_clear_cells);
        register_test(test_whole_column_reference);
        register_test(test_whole_row_reference);
        register_test(test_mixed_reference_formats);
        register_test(test_ref_error_handling);
    }

    void test_construction()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();

        xlnt::range range_1(ws, xlnt::range_reference("A1:D10"));
        xlnt_assert_equals(range_1.target_worksheet(), ws);
        xlnt_assert_equals(1, range_1.front()[0].row()); // NOTE: querying row/column here desperately needs some shortcuts
        xlnt_assert_equals(xlnt::column_t("D"), range_1.front().back().column());
        xlnt_assert_equals(10, range_1.back()[0].row());
        xlnt_assert_equals(xlnt::column_t("D"), range_1.back().back().column());
        // assert default parameters in ctor
        xlnt::range range_2(ws, xlnt::range_reference("A1:D10"), xlnt::major_order::row, false);
        xlnt_assert_equals(range_1, range_2);
        // assert copy
        xlnt::range range_3(range_2);
        xlnt_assert_equals(range_1, range_3);

        // column order
        xlnt::range range_4(ws, xlnt::range_reference("A1:D10"), xlnt::major_order::column);
        xlnt_assert_equals(xlnt::column_t("A"), range_4.front()[0].column()); // NOTE: querying row/column here desperately needs some shortcuts
        xlnt_assert_equals(10, range_4.front().back().row());
        xlnt_assert_equals(xlnt::column_t("D"), range_4.back()[0].column());
        xlnt_assert_equals(10, range_4.back().back().row());
        // assignment
        range_3 = range_4;
        xlnt_assert_equals(range_3, range_4);
    }

    void test_batch_formatting()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();

        for (auto row = 1; row <= 10; ++row)
        {
            for (auto column = 1; column <= 10; ++column)
            {
                auto ref = xlnt::cell_reference(column, row);
                ws[ref].value(ref.to_string());
            }
        }

        ws.range("A1:A10").font(xlnt::font().name("Arial"));
        ws.range("A1:J1").font(xlnt::font().bold(true));

        xlnt_assert_equals(ws.cell("A1").font().name(), "Calibri");
        xlnt_assert(ws.cell("A1").font().bold());

        xlnt_assert_equals(ws.cell("A2").font().name(), "Arial");
        xlnt_assert(!ws.cell("A2").font().bold());

        xlnt_assert_equals(ws.cell("B1").font().name(), "Calibri");
        xlnt_assert(ws.cell("B1").font().bold());

        xlnt_assert(!ws.cell("B2").has_format());
    }

    void test_clear_cells()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        ws.cell("A1").value("A1");
        ws.cell("A3").value("A3");
        ws.cell("C1").value("C1");
        ws.cell("B2").value("B2");
        ws.cell("C3").value("C3");
        xlnt_assert_equals(ws.calculate_dimension(), xlnt::range_reference(1, 1, 3, 3));
        auto range = ws.range("B1:C3");
        range.clear_cells();
        xlnt_assert_equals(ws.calculate_dimension(), xlnt::range_reference(1, 1, 1, 3));
    }

    void test_whole_column_reference()
    {
        // Test parsing of whole column references like "A:C" and "$A:$C"
        xlnt::range_reference ref1("A:C");
        xlnt_assert_equals(ref1.top_left().column(), xlnt::column_t("A"));
        xlnt_assert_equals(ref1.top_left().row(), 1);
        xlnt_assert_equals(ref1.bottom_right().column(), xlnt::column_t("C"));
        xlnt_assert_equals(ref1.bottom_right().row(), xlnt::constants::max_row());

        xlnt::range_reference ref2("$A:$C");
        xlnt_assert_equals(ref2.top_left().column(), xlnt::column_t("A"));
        xlnt_assert_equals(ref2.top_left().row(), 1);
        xlnt_assert_equals(ref2.bottom_right().column(), xlnt::column_t("C"));
        xlnt_assert_equals(ref2.bottom_right().row(), xlnt::constants::max_row());

        // Test single column reference
        xlnt::range_reference ref3("B:B");
        xlnt_assert_equals(ref3.top_left().column(), xlnt::column_t("B"));
        xlnt_assert_equals(ref3.top_left().row(), 1);
        xlnt_assert_equals(ref3.bottom_right().column(), xlnt::column_t("B"));
        xlnt_assert_equals(ref3.bottom_right().row(), xlnt::constants::max_row());

        // Test with workbook and worksheet
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto range = ws.range("A:C");
        xlnt_assert_equals(range.target_worksheet(), ws);
    }

    void test_whole_row_reference()
    {
        // Test parsing of whole row references like "1:5" and "$1:$5"
        xlnt::range_reference ref1("1:5");
        xlnt_assert_equals(ref1.top_left().column(), xlnt::constants::min_column());
        xlnt_assert_equals(ref1.top_left().row(), 1);
        xlnt_assert_equals(ref1.bottom_right().column(), xlnt::constants::max_column());
        xlnt_assert_equals(ref1.bottom_right().row(), 5);

        xlnt::range_reference ref2("$1:$5");
        xlnt_assert_equals(ref2.top_left().column(), xlnt::constants::min_column());
        xlnt_assert_equals(ref2.top_left().row(), 1);
        xlnt_assert_equals(ref2.bottom_right().column(), xlnt::constants::max_column());
        xlnt_assert_equals(ref2.bottom_right().row(), 5);

        // Test single row reference
        xlnt::range_reference ref3("3:3");
        xlnt_assert_equals(ref3.top_left().column(), xlnt::constants::min_column());
        xlnt_assert_equals(ref3.top_left().row(), 3);
        xlnt_assert_equals(ref3.bottom_right().column(), xlnt::constants::max_column());
        xlnt_assert_equals(ref3.bottom_right().row(), 3);

        // Test with workbook and worksheet
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto range = ws.range("1:5");
        xlnt_assert_equals(range.target_worksheet(), ws);
    }

    void test_mixed_reference_formats()
    {
        // Test that normal cell references still work
        xlnt::range_reference ref1("A1:C5");
        xlnt_assert_equals(ref1.top_left().column(), xlnt::column_t("A"));
        xlnt_assert_equals(ref1.top_left().row(), 1);
        xlnt_assert_equals(ref1.bottom_right().column(), xlnt::column_t("C"));
        xlnt_assert_equals(ref1.bottom_right().row(), 5);

        // Test single cell reference
        xlnt::range_reference ref2("B2");
        xlnt_assert_equals(ref2.top_left().column(), xlnt::column_t("B"));
        xlnt_assert_equals(ref2.top_left().row(), 2);
        xlnt_assert_equals(ref2.bottom_right().column(), xlnt::column_t("B"));
        xlnt_assert_equals(ref2.bottom_right().row(), 2);

        // Test absolute references
        xlnt::range_reference ref3("$A$1:$C$5");
        xlnt_assert_equals(ref3.top_left().column(), xlnt::column_t("A"));
        xlnt_assert_equals(ref3.top_left().row(), 1);
        xlnt_assert_equals(ref3.bottom_right().column(), xlnt::column_t("C"));
        xlnt_assert_equals(ref3.bottom_right().row(), 5);
    }

    void test_ref_error_handling()
    {
        // Test handling of #REF! error references
        // These should not throw exceptions but create valid range objects
        xlnt_assert_throws_nothing(xlnt::range_reference("#REF!"));

        xlnt::range_reference ref_error("#REF!");
        xlnt_assert_equals(ref_error.top_left().column(), xlnt::column_t("A"));
        xlnt_assert_equals(ref_error.top_left().row(), 1);
        xlnt_assert_equals(ref_error.bottom_right().column(), xlnt::column_t("A"));
        xlnt_assert_equals(ref_error.bottom_right().row(), 1);

        // Test ranges with #REF! in parts
        xlnt_assert_throws_nothing(xlnt::range_reference("#REF!:A5"));
        xlnt_assert_throws_nothing(xlnt::range_reference("A1:#REF!"));
        xlnt_assert_throws_nothing(xlnt::range_reference("#REF!:#REF!"));
    }
};
static range_test_suite x;
