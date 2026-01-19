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

#include <sstream>

#include <helpers/assertions.hpp>
#include <helpers/test_suite.hpp>

#include <xlnt/cell/cell.hpp>
#include <xlnt/cell/comment.hpp>
#include <xlnt/cell/hyperlink.hpp>
#include <xlnt/styles/alignment.hpp>
#include <xlnt/styles/border.hpp>
#include <xlnt/styles/fill.hpp>
#include <xlnt/styles/format.hpp>
#include <xlnt/styles/number_format.hpp>
#include <xlnt/styles/protection.hpp>
#include <xlnt/styles/style.hpp>
#include <xlnt/utils/date.hpp>
#include <xlnt/utils/datetime.hpp>
#include <xlnt/utils/time.hpp>
#include <xlnt/utils/timedelta.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/range.hpp>
#include <xlnt/worksheet/worksheet.hpp>

class cell_test_suite : public test_suite
{
public:
    cell_test_suite()
    {
        register_test(test_infer_numeric);
        register_test(test_constructor);
        register_test(test_null);
        register_test(test_string);
        register_test(test_formula1);
        register_test(test_formula2);
        register_test(test_formula3);
        register_test(test_not_formula);
        register_test(test_boolean);
        register_test(test_error_codes);
        register_test(test_insert_datetime);
        register_test(test_insert_date);
        register_test(test_insert_time);
        register_test(test_cell_formatted_as_date1);
        register_test(test_cell_formatted_as_date2);
        register_test(test_cell_formatted_as_date3);
        register_test(test_illegal_characters);
        register_test(test_timedelta);
        register_test(test_cell_offset);
        register_test(test_format);
        register_test(test_font);
        register_test(test_fill);
        register_test(test_border);
        register_test(test_number_format);
        register_test(test_number_format_merge);
        register_test(test_alignment);
        register_test(test_protection);
        register_test(test_style);
        register_test(test_print);
        register_test(test_values);
        register_test(test_reference);
        register_test(test_anchor);
        register_test(test_hyperlink);
        register_test(test_comment);
        register_test(test_copy_and_compare);
        register_test(test_copy_value_between_workbooks);
        register_test(test_copy_inline_string_between_workbooks);
        register_test(test_copy_formula_string_between_workbooks);
        register_test(test_format_from_different_workbook);
        register_test(test_cell_phonetic_properties);
    }

private:
    void test_infer_numeric()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        cell.value("4.2", true);
        xlnt_assert_delta(cell.value<double>(), 4.2, 1E-9);

        cell.value("-42.000", true);
        xlnt_assert(cell.value<int>() == -42);

        cell.value("0", true);
        xlnt_assert(cell.value<int>() == 0);

        cell.value("0.9999", true);
        xlnt_assert(cell.value<double>() == 0.9999);

        cell.value("99E-02", true);
        xlnt_assert(cell.value<double>() == 0.99);

        cell.value("4", true);
        xlnt_assert(cell.value<int>() == 4);

        cell.value("-1E3", true);
        xlnt_assert(cell.value<int>() == -1000);

        cell.value("2e+2", true);
        xlnt_assert(cell.value<int>() == 200);

        cell.value("3.1%", true);
        xlnt_assert_delta(cell.value<double>(), 0.031, 1E-9);

        cell.value("03:40:16", true);
        xlnt_assert(cell.value<xlnt::time>() == xlnt::time(3, 40, 16));

        cell.value("03:", true);
        xlnt_assert_equals(cell.value<std::string>(), "03:");

        cell.value("03:40", true);
        xlnt_assert(cell.value<xlnt::time>() == xlnt::time(3, 40));

        cell.value("30:33.865633336", true);
        xlnt_assert(cell.value<xlnt::time>() == xlnt::time(0, 30, 33, 865633));
    }

    void test_constructor()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference("A", 1));

        xlnt_assert(cell.data_type() == xlnt::cell::type::empty);
        xlnt_assert(cell.column() == "A");
        xlnt_assert(cell.column_index() == 1);
        xlnt_assert(cell.row() == 1);
        xlnt_assert(cell.reference() == "A1");
        xlnt_assert(!cell.has_value());
    }

    void test_null()
    {
        xlnt::workbook wb;

        const auto datatypes =
            {
                xlnt::cell::type::empty,
                xlnt::cell::type::boolean,
                xlnt::cell::type::error,
                xlnt::cell::type::formula_string,
                xlnt::cell::type::number,
                xlnt::cell::type::shared_string};

        for (const auto &datatype : datatypes)
        {
            auto ws = wb.active_sheet();
            auto cell = ws.cell(xlnt::cell_reference(1, 1));

            cell.data_type(datatype);
            xlnt_assert(cell.data_type() == datatype);
            cell.clear_value();
            xlnt_assert(cell.data_type() == xlnt::cell::type::empty);
        }
    }

    void test_string()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value("hello");
        xlnt_assert(cell.data_type() == xlnt::cell::type::shared_string);

        cell.value(".");
        xlnt_assert(cell.data_type() == xlnt::cell::type::shared_string);

        cell.value("0800");
        xlnt_assert(cell.data_type() == xlnt::cell::type::shared_string);
    }

    void test_formula1()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value("=42", true);
        //xlnt_assert(cell.data_type() == xlnt::cell::type::number);
        xlnt_assert(cell.has_formula());
    }

    void test_formula2()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value("=if(A1<4;-1;1)", true);
        //xlnt_assert(cell.data_type() == xlnt::cell::type::number);
        xlnt_assert(cell.has_formula());
    }

    void test_formula3()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        xlnt_assert(!cell.has_formula());
        xlnt_assert_throws(cell.formula(), xlnt::invalid_attribute);
        xlnt_assert_throws_nothing(cell.formula(""));
        xlnt_assert(!cell.has_formula());
        cell.formula("=42");
        xlnt_assert(cell.has_formula());
        xlnt_assert_equals(cell.formula(), "42");
        cell.clear_formula();
        xlnt_assert(!cell.has_formula());
        xlnt_assert_throws(cell.formula(), xlnt::invalid_attribute);
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.clear_formula());
    }

    void test_not_formula()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value("=", true);
        xlnt_assert(cell.data_type() == xlnt::cell::type::shared_string);
        xlnt_assert(cell.value<std::string>() == "=");
        xlnt_assert(!cell.has_formula());
        xlnt_assert_throws(cell.formula(), xlnt::invalid_attribute);
    }

    void test_boolean()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        for (auto value : {true, false})
        {
            cell.value(value);
            xlnt_assert(cell.data_type() == xlnt::cell::type::boolean);
        }
    }

    void test_error_codes()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));
        // error string can't be empty
        xlnt_assert_throws(cell.error(""), xlnt::exception);
        // error string has to have a leading '#'
        xlnt_assert_throws(cell.error("not an error"), xlnt::exception);

        for (auto error_code : xlnt::cell::error_codes())
        {
            // error type from the string format
            cell.value(error_code.first, true);
            xlnt_assert(cell.data_type() == xlnt::cell::type::error);
            std::string error;
            xlnt_assert_throws_nothing(error = cell.error());
            xlnt_assert_equals(error, error_code.first);
            // clearing the value clears the error state
            cell.clear_value();
            xlnt_assert_throws(cell.error(), xlnt::invalid_attribute);
            // can explicitly set the error
            xlnt_assert_throws_nothing(cell.error(error_code.first));
            std::string error2;
            xlnt_assert_throws_nothing(error2 = cell.error());
            xlnt_assert_equals(error2, error_code.first);
        }
    }

    void test_insert_datetime()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::datetime(2010, 7, 13, 6, 37, 41));

        xlnt_assert(cell.data_type() == xlnt::cell::type::number);
        xlnt_assert_delta(cell.value<double>(), 40372.27616898148, 1E-9);
        xlnt_assert(cell.is_date());
        xlnt_assert(cell.number_format().format_string() == "yyyy-mm-dd h:mm:ss");
    }

    void test_insert_date()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::date(2010, 7, 13));
        xlnt_assert(cell.data_type() == xlnt::cell::type::number);
        xlnt_assert(cell.value<double>() == 40372.0);
        xlnt_assert(cell.is_date());
        xlnt_assert(cell.number_format().format_string() == "yyyy-mm-dd");
    }

    void test_insert_time()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::time(1, 3));
        xlnt_assert(cell.data_type() == xlnt::cell::type::number);
        xlnt_assert_delta(cell.value<double>(), 0.04375, 1E-9);
        xlnt_assert(cell.is_date());
        xlnt_assert(cell.number_format().format_string() == "h:mm:ss");
    }

    void test_cell_formatted_as_date1()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::datetime::today());
        cell.clear_value();
        xlnt_assert(!cell.is_date()); // disagree with openpyxl
        xlnt_assert(!cell.has_value());
    }

    void test_cell_formatted_as_date2()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::datetime::today());
        cell.value("testme");
        xlnt_assert(!cell.is_date());
        xlnt_assert(cell.value<std::string>() == "testme");
    }

    void test_cell_formatted_as_date3()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::datetime::today());
        cell.value(true);
        xlnt_assert(!cell.is_date());
        xlnt_assert(cell.value<bool>() == true);
    }

    void test_illegal_characters()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        // The bytes 0x00 through 0x1F inclusive must be manually escaped in values.
        auto illegal_chrs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 14, 15, 16, 17, 18,
            19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

        for (auto i : illegal_chrs)
        {
            std::string str(1, i);
            xlnt_assert_throws(cell.value(str), xlnt::illegal_character);
        }

        cell.value(std::string(1, 33));
        cell.value(std::string(1, 9)); // Tab
        cell.value(std::string(1, 10)); // Newline
        cell.value(std::string(1, 13)); // Carriage return
        cell.value(" Leading and trailing spaces are legal ");
    }

    // void test_time_regex() {}

    void test_timedelta()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        cell.value(xlnt::timedelta(1, 3, 0, 0, 0));

        xlnt_assert(cell.value<double>() == 1.125);
        xlnt_assert(cell.data_type() == xlnt::cell::type::number);
        xlnt_assert(!cell.is_date());
        xlnt_assert(cell.number_format().format_string() == "[hh]:mm:ss");
    }

    void test_cell_offset()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));
        xlnt_assert(cell.offset(1, 2).reference() == "B3");
    }

    void test_format()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xlnt::cell_reference(1, 1));

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws(cell.format(), xlnt::invalid_attribute);

        xlnt::format format = wb.create_format();
        cell.format(format);

        xlnt_assert(cell.has_format());
        xlnt_assert_throws_nothing(cell.format());
        cell.clear_format();
        xlnt_assert(!cell.has_format());
        xlnt_assert_throws(cell.format(), xlnt::invalid_attribute);
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.clear_format());
    }

    void test_font()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws_nothing(cell.font());

        auto font = xlnt::font().bold(true);

        cell.font(font);

        xlnt_assert(cell.has_format());
        xlnt_assert(cell.format().has_font());
        xlnt_assert(cell.format().font_applied());
        xlnt_assert_equals(cell.font(), font);
    }

    void test_fill()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws_nothing(cell.fill());

        xlnt::fill fill(xlnt::pattern_fill()
                            .type(xlnt::pattern_fill_type::solid)
                            .foreground(xlnt::color::red()));
        cell.fill(fill);

        xlnt_assert(cell.has_format());
        xlnt_assert(cell.format().has_fill());
        xlnt_assert(cell.format().fill_applied());
        xlnt_assert_equals(cell.fill(), fill);
    }

    void test_border()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws_nothing(cell.border());

        xlnt::border border;
        cell.border(border);

        xlnt_assert(cell.has_format());
        xlnt_assert(cell.format().has_border());
        xlnt_assert(cell.format().border_applied());
        xlnt_assert_equals(cell.border(), border);
    }

    void test_number_format()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws_nothing(cell.number_format());

        xlnt::number_format format("dd--hh--mm");
        cell.number_format(format);

        xlnt_assert(cell.has_format());
        xlnt_assert(cell.format().has_number_format());
        xlnt_assert(cell.format().number_format_applied());
        xlnt_assert_equals(cell.number_format(), format);
        xlnt_assert_equals(cell.number_format().format_string(), "dd--hh--mm");
    }

    void test_number_format_merge()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell_fmt_1a = ws.cell("A1");
        auto cell_fmt_2a = ws.cell("A2");
        auto cell_fmt_1b = ws.cell("A3");
        auto cell_fmt_2b = ws.cell("A4");

        xlnt::number_format format_1a("DD-MM-YYYY hh:mm:ss");
        xlnt::number_format format_2a("dd--hh--mm");
        xlnt::number_format format_1b("DD-MM-YYYY hh:mm:ss");
        xlnt::number_format format_2b("dd--hh--mm");

        cell_fmt_1a.number_format(format_1a);
        cell_fmt_2a.number_format(format_2a);
        cell_fmt_1b.number_format(format_1b);
        cell_fmt_2b.number_format(format_2b);

        xlnt_assert_equals(cell_fmt_1a.number_format().id(), cell_fmt_1b.number_format().id());
        xlnt_assert_equals(cell_fmt_2a.number_format().id(), cell_fmt_2b.number_format().id());

        xlnt_assert_differs(cell_fmt_1a.number_format().id(), cell_fmt_2a.number_format().id());
        xlnt_assert_differs(cell_fmt_1b.number_format().id(), cell_fmt_2b.number_format().id());
    }

    void test_alignment()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws_nothing(cell.alignment());

        xlnt::alignment align;
        align.wrap(true);

        cell.alignment(align);

        xlnt_assert(cell.has_format());
        xlnt_assert(cell.format().has_alignment());
        xlnt_assert(cell.format().alignment_applied());
        xlnt_assert_equals(cell.alignment(), align);
    }

    void test_protection()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_format());
        xlnt_assert_throws_nothing(cell.protection());

        auto protection = xlnt::protection().locked(false).hidden(true);
        cell.protection(protection);

        xlnt_assert(cell.has_format());
        xlnt_assert(cell.format().has_protection());
        xlnt_assert(cell.format().protection_applied());
        xlnt_assert_equals(cell.protection(), protection);
    }

    void test_style()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xlnt_assert(!cell.has_style());
        xlnt_assert_throws(cell.style(), xlnt::invalid_attribute);

        auto test_style = wb.create_style("test_style");
        test_style.number_format(xlnt::number_format::date_ddmmyyyy(), true);

        // Style for testing all properties
        auto style_all = wb.create_style("style_all");
        xlnt::alignment alignment_all;
        alignment_all.horizontal(xlnt::horizontal_alignment::center_continuous);
        alignment_all.vertical(xlnt::vertical_alignment::distributed);
        alignment_all.rotation(45);
        alignment_all.indent(10);
        alignment_all.shrink(true);
        alignment_all.wrap(true);
        style_all.alignment(alignment_all);
        xlnt::border border_all;
        border_all.diagonal(xlnt::diagonal_direction::both);
        style_all.border(border_all);
        xlnt::gradient_fill gradient_fill_all;
        gradient_fill_all.left(5);
        gradient_fill_all.right(10);
        gradient_fill_all.add_stop(6.5, xlnt::color::yellow());
        xlnt::fill fill_all(gradient_fill_all);
        style_all.fill(fill_all);
        xlnt::font font_all;
        font_all.bold(true);
        font_all.underline(xlnt::font::underline_style::double_accounting);
        style_all.font(font_all);
        xlnt::number_format number_format_all;
        number_format_all.id(999);
        number_format_all.format_string("yyyy\\mm\\dd"); // cursed date format
        style_all.number_format(number_format_all);
        xlnt::protection protection_all;
        protection_all.hidden(true);
        protection_all.locked(true);
        style_all.protection(protection_all);
        cell.style(style_all);
        xlnt_assert_equals(cell.style(), style_all);
        xlnt_assert_equals(cell.alignment(), alignment_all);
        xlnt_assert_equals(cell.border(), border_all);
        xlnt_assert_equals(cell.fill(), fill_all);
        xlnt_assert_equals(cell.font(), font_all);
        xlnt_assert_equals(cell.number_format(), number_format_all);
        xlnt_assert_equals(cell.protection(), protection_all);

        cell.style(test_style);
        xlnt_assert(cell.has_style());
        xlnt_assert_equals(cell.style().number_format(), xlnt::number_format::date_ddmmyyyy());
        xlnt_assert_equals(cell.style(), test_style);

        auto other_style = wb.create_style("other_style");
        other_style.number_format(xlnt::number_format::date_time2(), true);

        cell.style("other_style");
        xlnt_assert_equals(cell.style().number_format(), xlnt::number_format::date_time2());
        xlnt_assert_equals(cell.style(), other_style);

        auto last_style = wb.create_style("last_style");
        last_style.number_format(xlnt::number_format::percentage(), true);

        cell.style(last_style);
        xlnt_assert_equals(cell.style().number_format(), xlnt::number_format::percentage());
        xlnt_assert_equals(cell.style(), last_style);

        xlnt_assert(!wb.has_style("doesn't exist"));
        xlnt_assert_throws(cell.style("doesn't exist"), xlnt::key_not_found);

        cell.clear_style();

        xlnt_assert(!cell.has_style());
        xlnt_assert_throws(cell.style(), xlnt::invalid_attribute);
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.clear_style());
    }

    void test_print()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();

        {
            auto cell = ws.cell("A1");

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xlnt_assert_equals(cell.to_string(), stream_string);
            xlnt_assert_equals(stream_string, "");
        }

        {
            auto cell = ws.cell("A2");

            cell.value(false);

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xlnt_assert_equals(cell.to_string(), stream_string);
            xlnt_assert_equals(stream_string, "FALSE");
        }

        {
            auto cell = ws.cell("A3");

            cell.value(true);

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xlnt_assert_equals(cell.to_string(), stream_string);
            xlnt_assert_equals(stream_string, "TRUE");
        }

        {
            auto cell = ws.cell("A4");

            cell.value(1.234);

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xlnt_assert_equals(cell.to_string(), stream_string);
            xlnt_assert_equals(stream_string, "1.234");
        }

        {
            auto cell = ws.cell("A5");

            cell.error("#REF");

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xlnt_assert_equals(cell.to_string(), stream_string);
            xlnt_assert_equals(stream_string, "#REF");
        }

        {
            auto cell = ws.cell("A6");

            cell.value("test");

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xlnt_assert_equals(cell.to_string(), stream_string);
            xlnt_assert_equals(stream_string, "test");
        }
    }

    void test_values()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        cell.value(static_cast<int>(4));
        xlnt_assert_equals(cell.value<int>(), 4);

        cell.value(static_cast<unsigned int>(3));
        xlnt_assert_equals(cell.value<unsigned>(), 3);

        cell.value(static_cast<unsigned long long>(4));
        xlnt_assert_equals(cell.value<unsigned long long>(), 4);

        cell.value(static_cast<long long int>(3));
        xlnt_assert_equals(cell.value<long long int>(), 3);

        cell.value(static_cast<float>(3.14));
        xlnt_assert_delta(cell.value<float>(), 3.14, 0.001);

        cell.value(static_cast<double>(3.141592));
        xlnt_assert_equals(cell.value<double>(), 3.141592);

        auto cell2 = ws.cell("A2");
        cell2.value(std::string(100000, 'a'));
        cell.value(cell2);
        xlnt_assert_equals(cell.value<std::string>(), std::string(32767, 'a'));
    }

    void test_reference()
    {
        xlnt::cell_reference_hash hash;
        xlnt_assert_differs(hash(xlnt::cell_reference("A2")), hash(xlnt::cell_reference(1, 1)));
        xlnt_assert_equals(hash(xlnt::cell_reference("A2")), hash(xlnt::cell_reference(1, 2)));

        xlnt_assert_equals((xlnt::cell_reference("A1"), xlnt::cell_reference("B2")), xlnt::range_reference("A1:B2"));

        xlnt_assert_throws(xlnt::cell_reference("A1&"), xlnt::invalid_cell_reference);
        xlnt_assert_throws(xlnt::cell_reference("A"), xlnt::invalid_cell_reference);

        auto ref = xlnt::cell_reference("$B$7");
        xlnt_assert_equals(ref.row_absolute(), true);
        xlnt_assert_equals(ref.column_absolute(), true);

        ref = xlnt::cell_reference("$B7");
        xlnt_assert_equals(ref.row_absolute(), false);
        xlnt_assert_equals(ref.column_absolute(), true);

        ref = xlnt::cell_reference("B$7");
        xlnt_assert_equals(ref.row_absolute(), true);
        xlnt_assert_equals(ref.column_absolute(), false);

        xlnt_assert(xlnt::cell_reference("A1") == "A1");
        xlnt_assert(xlnt::cell_reference("A1") != "A2");
    }

    void test_anchor()
    {
        xlnt::workbook wb;
        auto cell = wb.active_sheet().cell("A1");
        auto anchor = cell.anchor();
        xlnt_assert_equals(anchor.first, 0);
        xlnt_assert_equals(anchor.second, 0);
    }

    void test_hyperlink()
    {
        xlnt::workbook wb;
        auto cell = wb.active_sheet().cell("A1");

        xlnt_assert(!cell.has_hyperlink());
        xlnt_assert_throws(cell.hyperlink(), xlnt::invalid_attribute);
        xlnt_assert_throws(cell.hyperlink(""), xlnt::invalid_parameter);
        // link without optional display
        const std::string link1("http://example.com");
        cell.hyperlink(link1);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert_throws_nothing(cell.hyperlink());
        xlnt_assert(cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().url(), link1);
        xlnt_assert_equals(cell.hyperlink().relationship().target().to_string(), link1);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), link1);
        cell.hyperlink().clear_display();
        xlnt_assert(!cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), "");
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.hyperlink().clear_display());
        cell.clear_value();
        // link with display
        const std::string link2("http://example2.com");
        const std::string display_txt("notaurl");
        cell.hyperlink(link2, display_txt);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert_throws_nothing(cell.hyperlink());
        xlnt_assert(cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().url(), link2);
        xlnt_assert_equals(cell.hyperlink().relationship().target().to_string(), link2);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), display_txt);
        xlnt_assert_throws(cell.hyperlink().target_range(), xlnt::invalid_attribute);
        cell.hyperlink().clear_display();
        xlnt_assert(!cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), "");
        // relative (local) url
        const std::string local("../test_local");
        cell.hyperlink(local);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert_throws_nothing(cell.hyperlink());
        xlnt_assert(cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().url(), local);
        xlnt_assert_equals(cell.hyperlink().relationship().target().to_string(), local);
        xlnt_assert_throws(cell.hyperlink().target_range(), xlnt::invalid_attribute);
        // value
        int cell_test_val = 123;
        cell.value(cell_test_val);
        std::string cell_value_str = std::to_string(cell_test_val);
        cell.hyperlink(link2, display_txt);
        xlnt_assert_equals(cell.value<int>(), 123);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), cell_value_str); // display text ignored
        cell.clear_value();
        // cell overload without display
        const std::string cell_target_str("A2");
        auto cell_target = wb.active_sheet().cell(cell_target_str);
        std::string link3 = wb.active_sheet().title() + '!' + cell_target_str; // Sheet1!A2
        cell.hyperlink(cell_target);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert(!cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().target_range(), link3);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), link3);
        xlnt_assert_throws(cell.hyperlink().relationship(), xlnt::invalid_attribute);
        xlnt_assert_throws(cell.hyperlink().url(), xlnt::invalid_attribute);
        cell.clear_value();
        // cell overload with display
        cell.hyperlink(cell_target, display_txt);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert(!cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().target_range(), link3);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), display_txt);
        xlnt_assert_throws(cell.hyperlink().relationship(), xlnt::invalid_attribute);
        xlnt_assert_throws(cell.hyperlink().url(), xlnt::invalid_attribute);
        // value
        cell.value(cell_test_val);
        cell.hyperlink(cell_target, display_txt);
        xlnt_assert_equals(cell.value<int>(), 123);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), cell_value_str); // display text ignored
        cell.clear_value();
        // range overload without display
        const std::string range_target_str("A2:A5");
        xlnt::range range_target(wb.active_sheet(), xlnt::range_reference(range_target_str));
        std::string link4 = wb.active_sheet().title() + '!' + range_target_str; // Sheet1!A2:A5
        cell.hyperlink(range_target);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert(!cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().target_range(), link4);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), link4);
        xlnt_assert_throws(cell.hyperlink().relationship(), xlnt::invalid_attribute);
        xlnt_assert_throws(cell.hyperlink().url(), xlnt::invalid_attribute);
        cell.clear_value();
        // range overload with display
        cell.hyperlink(range_target, display_txt);
        xlnt_assert(cell.has_hyperlink());
        xlnt_assert(!cell.hyperlink().external());
        xlnt_assert_equals(cell.hyperlink().target_range(), link4);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), display_txt);
        xlnt_assert_throws(cell.hyperlink().relationship(), xlnt::invalid_attribute);
        xlnt_assert_throws(cell.hyperlink().url(), xlnt::invalid_attribute);
        // value
        cell.value(cell_test_val);
        cell.hyperlink(range_target, display_txt);
        xlnt_assert_equals(cell.value<int>(), 123);
        xlnt_assert(cell.hyperlink().has_display());
        xlnt_assert_equals(cell.hyperlink().display(), cell_value_str); // display text ignored
        cell.clear_value();
        // tooltip
        cell.hyperlink(link1);
        xlnt_assert(!cell.hyperlink().has_tooltip());
        xlnt_assert_equals(cell.hyperlink().tooltip(), "");
        cell.hyperlink().tooltip("example");
        xlnt_assert(cell.hyperlink().has_tooltip());
        xlnt_assert_equals(cell.hyperlink().tooltip(), "example");
        cell.hyperlink().clear_tooltip();
        xlnt_assert(!cell.hyperlink().has_tooltip());
        xlnt_assert_equals(cell.hyperlink().tooltip(), "");
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.hyperlink().clear_tooltip());
        // location
        cell.hyperlink(link1);
        xlnt_assert(!cell.hyperlink().has_location());
        xlnt_assert_equals(cell.hyperlink().location(), "");
        cell.hyperlink().location("location");
        xlnt_assert(cell.hyperlink().has_location());
        xlnt_assert_equals(cell.hyperlink().location(), "location");
        cell.hyperlink().clear_location();
        xlnt_assert(!cell.hyperlink().has_location());
        xlnt_assert_equals(cell.hyperlink().location(), "");
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.hyperlink().clear_location());
    }

    void test_comment()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");
        xlnt_assert(!cell.has_comment());
        xlnt_assert_throws(cell.comment(), xlnt::invalid_attribute);
        cell.comment(xlnt::comment("comment", "author"));
        xlnt_assert(cell.has_comment());
        xlnt_assert_equals(cell.comment(), xlnt::comment("comment", "author"));
        cell.clear_comment();
        xlnt_assert(!cell.has_comment());
        xlnt_assert_throws(cell.comment(), xlnt::invalid_attribute);
        // Clearing again should never throw.
        xlnt_assert_throws_nothing(cell.clear_comment());

        xlnt::comment comment_with_size("test comment", "author");
        comment_with_size.size(1000, 30);
        cell.comment(comment_with_size);
        xlnt_assert_equals(cell.comment(), comment_with_size);
    }

    void test_copy_and_compare()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell1 = ws.cell("A1");
        auto cell2 = ws.cell("A2");

        xlnt_assert_equals(cell1, cell1);
        xlnt_assert_equals(cell2, cell2);
        xlnt_assert_differs(cell1, cell2);
        xlnt_assert_differs(cell2, cell1);
        // nullptr equality
        xlnt_assert(nullptr == cell1);
        xlnt_assert(cell1 == nullptr);
        cell1.value("test");
        xlnt_assert(!(nullptr == cell1));
        xlnt_assert(!(cell1 == nullptr));
        // copy
        xlnt::cell cell3(cell1);
        xlnt_assert_equals(cell1, cell3);
        // assign
        cell3 = cell2;
        xlnt_assert_equals(cell2, cell3);
    }

    void test_copy_value_between_workbooks()
    {
        // Test 1: Cross-workbook copy with scope destruction (proves deep copy, not raw pointers)
        xlnt::workbook wb_dest;
        auto cell_dest = wb_dest.active_sheet().cell("A1");

        {
            xlnt::workbook wb_source;
            auto cell_source = wb_source.active_sheet().cell("A1");

            // Set various value types
            cell_source.value("Test String");
            cell_source.formula("=SUM(A1:A10)");
            cell_source.font(xlnt::font().bold(true).size(14));
            // External hyperlink should be copied on cross-workbook copy
            cell_source.hyperlink("https://example.com");

            cell_dest.value(cell_source);
        }
        // Source workbook destroyed - if raw pointers were copied, this would crash/corrupt

        xlnt_assert_equals(cell_dest.value<std::string>(), "Test String");
        xlnt_assert(cell_dest.has_formula());
        xlnt_assert_equals(cell_dest.formula(), "SUM(A1:A10)");
        xlnt_assert(cell_dest.has_format());
        xlnt_assert(cell_dest.font().bold());
        xlnt_assert_equals(cell_dest.font().size(), 14.0);
        xlnt_assert(cell_dest.has_hyperlink());
        xlnt_assert_equals(cell_dest.hyperlink().url(), "https://example.com");

        // Test 2: Shared string deduplication
        xlnt::workbook wb_a;
        auto ws_a = wb_a.active_sheet();

        xlnt::workbook wb_b;
        auto ws_b = wb_b.active_sheet();

        ws_a.cell("A1").value("Duplicate");
        ws_a.cell("A2").value("Duplicate");

        ws_b.cell("B1").value(ws_a.cell("A1"));
        auto count_after_first = wb_b.shared_strings().size();

        ws_b.cell("B2").value(ws_a.cell("A2")); // Same string
        xlnt_assert_equals(wb_b.shared_strings().size(), count_after_first); // Deduplicated

        // Test 3: Same-workbook shallow copy (format_count unchanged, hyperlink preserved)
        xlnt::workbook wb_same;
        auto ws_same = wb_same.active_sheet();
        auto cell_src = ws_same.cell("A1");
        auto cell_dst = ws_same.cell("A2");

        cell_src.value("Data");
        cell_src.font(xlnt::font().italic(true));
        cell_src.hyperlink("https://example.com");

        auto format_count = wb_same.format_count();
        cell_dst.value(cell_src);

        xlnt_assert_equals(wb_same.format_count(), format_count); // No new format created
        xlnt_assert(cell_dst.has_hyperlink()); // Hyperlink preserved (same workbook)
        xlnt_assert_equals(cell_dst.hyperlink().url(), "https://example.com");
    }

    void test_copy_inline_string_between_workbooks()
    {
        xlnt::workbook wb_dest;
        auto cell_dest = wb_dest.active_sheet().cell("A1");
        std::string expected_value;
        xlnt::cell::type expected_type;

        {
            xlnt::workbook wb_source;
            wb_source.load(path_helper::test_file("Issue445_inline_str.xlsx"));
            auto cell_source = wb_source.active_sheet().cell("A1");

            expected_value = cell_source.value<std::string>();
            expected_type = cell_source.data_type();

            cell_dest.value(cell_source);
        }
        // Source workbook destroyed

        xlnt_assert_equals(cell_dest.value<std::string>(), expected_value);
        xlnt_assert_equals(cell_dest.data_type(), expected_type);
    }

    void test_copy_formula_string_between_workbooks()
    {
        xlnt::workbook wb_dest;
        auto cell_dest = wb_dest.active_sheet().cell("A1");
        std::string expected_value;
        xlnt::cell::type expected_type;

        {
            xlnt::workbook wb_source;
            wb_source.load(path_helper::test_file("18_formulae.xlsx"));
            auto cell_source = wb_source.sheet_by_index(0).cell("D1");

            expected_value = cell_source.value<std::string>();
            expected_type = cell_source.data_type();

            cell_dest.value(cell_source);
        }
        // Source workbook destroyed

        xlnt_assert_equals(cell_dest.value<std::string>(), expected_value);
        xlnt_assert_equals(cell_dest.data_type(), expected_type);
    }

    // Test cross-workbook format deep-copy via cell::format()
    void test_format_from_different_workbook()
    {
        // Test 1: Deep copy with scope destruction - proves no dangling pointers
        xlnt::workbook wb_dest;
        wb_dest.create_style("TestStyle"); // Pre-create style with same name
        auto cell_dest = wb_dest.active_sheet().cell("A1");

        auto initial_format_count = wb_dest.format_count();

        {
            xlnt::workbook wb_source;
            auto fmt = wb_source.create_format();

            fmt.font(xlnt::font().bold(true).size(14), true);
            fmt.fill(xlnt::fill(xlnt::pattern_fill()
                             .type(xlnt::pattern_fill_type::solid)
                             .foreground(xlnt::color::red())),
                true);
            fmt.border(xlnt::border().side(xlnt::border_side::top,
                           xlnt::border::border_property().style(xlnt::border_style::thick)),
                true);
            fmt.alignment(xlnt::alignment().horizontal(xlnt::horizontal_alignment::center), true);
            fmt.protection(xlnt::protection().locked(false), true);
            fmt.number_format(xlnt::number_format::percentage(), true);
            fmt.pivot_button(true);
            fmt.quote_prefix(true);
            wb_source.create_style("TestStyle");
            fmt.style("TestStyle");

            cell_dest.format(fmt);
        }
        // Source workbook destroyed - accessing format must not crash

        // Essential checks: format was cloned into destination workbook,
        // cell has a format, font bold preserved, and style associated by name.
        xlnt_assert(wb_dest.format_count() > initial_format_count);
        xlnt_assert(cell_dest.has_format());
        xlnt_assert(cell_dest.font().bold());
        xlnt_assert_delta(cell_dest.font().size(), 14.0, 1E-9);
        xlnt_assert(cell_dest.has_style());

        auto format = cell_dest.format();
        xlnt_assert_equals(format.style().name(), "TestStyle");
        xlnt_assert_equals(format.fill().pattern_fill().type(), xlnt::pattern_fill_type::solid);
        xlnt_assert_equals(format.fill().pattern_fill().foreground(), xlnt::color::red());
        xlnt_assert_equals(format.border().side(xlnt::border_side::top).get().style(), xlnt::border_style::thick);
        xlnt_assert_equals(format.alignment().horizontal(), xlnt::horizontal_alignment::center);
        xlnt_assert_equals(format.protection().locked(), false);
        xlnt_assert_equals(format.number_format(), xlnt::number_format::percentage());
        xlnt_assert_equals(format.pivot_button(), true);
        xlnt_assert_equals(format.quote_prefix(), true);

        // Test 2: Same workbook - no cloning (format_count unchanged)
        xlnt::workbook wb_same;
        auto cell_a = wb_same.active_sheet().cell("A1");
        auto cell_b = wb_same.active_sheet().cell("A2");

        cell_a.font(xlnt::font().italic(true));
        auto format_count = wb_same.format_count();

        cell_b.format(cell_a.format());

        xlnt_assert_equals(wb_same.format_count(), format_count);
        xlnt_assert(cell_b.font().italic());
    }

    void test_cell_phonetic_properties()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell1 = ws.cell("A1");

        xlnt_assert_equals(cell1.phonetics_visible(), false);
        cell1.show_phonetics(true);
        xlnt_assert_equals(cell1.phonetics_visible(), true);
        cell1.show_phonetics(false);
        xlnt_assert_equals(cell1.phonetics_visible(), false);
    }
};

static cell_test_suite x{};
