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

#include <algorithm>
#include <iostream>

#include <xlnt/xlnt.hpp>
#include <detail/serialization/open_stream.hpp>
#include <xlnt/utils/exceptions.hpp>
#include <helpers/temporary_file.hpp>
#include <helpers/test_suite.hpp>
#include <detail/constants.hpp>

#include <xlnt/cell/cell.hpp>
#include <xlnt/styles/format.hpp>
#include <xlnt/styles/style.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/workbook/worksheet_iterator.hpp>
#include <xlnt/worksheet/range.hpp>
#include <xlnt/worksheet/worksheet.hpp>

class workbook_test_suite : public test_suite
{
public:
    workbook_test_suite()
    {
        register_test(test_active_sheet);
        register_test(test_create_sheet);
        register_test(test_add_correct_sheet);
        register_test(test_add_sheet_from_other_workbook);
        register_test(test_add_sheet_at_index);
        register_test(test_get_sheet_by_title);
        register_test(test_get_sheet_by_title_const);
        register_test(test_get_sheet_by_index);
        register_test(test_get_sheet_by_index_const);
        register_test(test_index_operator);
        register_test(test_contains);
        register_test(test_iter);
        register_test(test_const_iter);
        register_test(test_get_index);
        register_test(test_get_sheet_names);
        register_test(test_add_named_range);
        register_test(test_get_named_range);
        register_test(test_remove_named_range);
        register_test(test_post_increment_iterator);
        register_test(test_clone);
        register_test(test_copy_constructor);
        register_test(test_copy_assignment_operator);
        register_test(test_copy_iterator);
        register_test(test_manifest);
        register_test(test_memory);
        register_test(test_clear);
        register_test(test_comparison);
        register_test(test_id_gen);
        register_test(test_load_file);
        register_test(test_load_file_encrypted);
        register_test(test_Issue279);
        register_test(test_Issue353);
        register_test(test_Issue494);
        register_test(test_Issue90);
        register_test(test_style);
        register_test(test_sheet_moving)
    }

    void test_active_sheet()
    {
        xlnt::workbook wb;
        xlnt_assert_equals(wb.active_sheet(), wb[0]);

        wb.create_sheet();
        wb.create_sheet();
        wb.active_sheet(2);
        xlnt_assert_equals(wb.active_sheet(), wb[2]);
    }

    void test_create_sheet()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        auto last = std::distance(wb.begin(), wb.end()) - 1;
        xlnt_assert_equals(new_sheet, wb[last]);
    }

    void test_add_correct_sheet()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        new_sheet.cell("A6").value(1.498);
        wb.copy_sheet(new_sheet);
        xlnt_assert_differs(wb[1].id(), wb[2].id());
        xlnt_assert(wb[1].compare(wb[2], false));
        wb.create_sheet().cell("A6").value(1.497);
        xlnt_assert(!wb[1].compare(wb[3], false));
    }

    void test_add_sheet_from_other_workbook()
    {
        xlnt::workbook wb1, wb2;
        auto new_sheet = wb1.active_sheet();
        xlnt_assert_throws(wb2.copy_sheet(new_sheet), xlnt::invalid_parameter);
        xlnt_assert_throws(wb2.index(new_sheet), std::runtime_error);
    }

    void test_add_sheet_at_index()
    {
        xlnt::workbook wb;
        auto ws = wb.active_sheet();
        ws.cell("B3").value(2);
        ws.title("Active");
        wb.copy_sheet(ws, 0);
        xlnt_assert_equals(wb.sheet_titles().at(0), "Sheet1");
        xlnt_assert_equals(wb.sheet_by_index(0).cell("B3").value<int>(), 2);
        xlnt_assert_equals(wb.sheet_titles().at(1), "Active");
        xlnt_assert_equals(wb.sheet_by_index(1).cell("B3").value<int>(), 2);
    }

    void test_remove_sheet()
    {
        xlnt::workbook wb, wb2;
        auto new_sheet = wb.create_sheet(0);
        new_sheet.title("removed");
        wb.remove_sheet(new_sheet);
        xlnt_assert(!wb.contains("removed"));
        xlnt_assert_throws(wb.remove_sheet(wb2.active_sheet()), std::runtime_error);
    }

    void test_get_sheet_by_title()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        std::string title = "my sheet";
        new_sheet.title(title);
        auto found_sheet = wb.sheet_by_title(title);
        xlnt_assert_equals(new_sheet, found_sheet);
        xlnt_assert_throws(wb.sheet_by_title("error"), xlnt::key_not_found);
        const auto &wb_const = wb;
        xlnt_assert_throws(wb_const.sheet_by_title("error"), xlnt::key_not_found);
    }

    void test_get_sheet_by_title_const()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        std::string title = "my sheet";
        new_sheet.title(title);
        const xlnt::workbook &wbconst = wb;
        auto found_sheet = wbconst.sheet_by_title(title);
        xlnt_assert_equals(new_sheet, found_sheet);
    }

    void test_get_sheet_by_index()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        xlnt_assert_equals(new_sheet, wb.sheet_by_index(1)); // in range
        xlnt_assert_throws(wb.sheet_by_index(2), xlnt::invalid_parameter); // out of range
    }

    void test_get_sheet_by_index_const()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        const auto &wb_const = wb;
        xlnt_assert_equals(new_sheet, wb_const.sheet_by_index(1)); // in range
        xlnt_assert_throws(wb_const.sheet_by_index(2), xlnt::invalid_parameter); // out of range
    }

    void test_index_operator() // test_getitem
    {
        xlnt::workbook wb;
        xlnt_assert_throws_nothing(wb["Sheet1"]);
        xlnt_assert_throws(wb["NotThere"], xlnt::key_not_found);
    }

    void test_contains()
    {
        xlnt::workbook wb;
        xlnt_assert(wb.contains("Sheet1"));
        xlnt_assert(!wb.contains("NotThere"));
    }

    void test_iter()
    {
        xlnt::workbook wb;

        for (auto ws : wb)
        {
            xlnt_assert_equals(ws.title(), "Sheet1");
        }

        xlnt::workbook wb2;
        auto iter = wb.begin();
        auto iter_copy(iter); // copy ctor
        xlnt_assert_equals(iter, iter_copy);
        auto begin_2(wb2.begin());
        xlnt_assert_differs(begin_2, iter);
        iter = begin_2; // copy assign
        xlnt_assert_equals(iter, begin_2);
        iter = wb.begin();
        iter = std::move(begin_2);
        xlnt_assert_equals(iter, wb2.begin());

        auto citer = wb.cbegin();
        auto citer_copy(citer); // copy ctor
        xlnt_assert_equals(citer, citer_copy);
        auto cbegin_2(wb2.cbegin());
        xlnt_assert_differs(cbegin_2, citer);
        citer = cbegin_2; // copy assign
        xlnt_assert_equals(citer, cbegin_2);
        citer = wb.cbegin();
        citer = std::move(cbegin_2);
        xlnt_assert_equals(citer, wb2.cbegin());

        wb2.create_sheet(); // wb2 iterators assumed invalidated
        iter = wb2.begin();
        xlnt_assert_equals((*iter).title(), "Sheet1");
        ++iter;
        xlnt_assert_differs(iter, wb2.begin());
        xlnt_assert_equals((*iter).title(), "Sheet2");
        auto temp = --iter;
        xlnt_assert_equals((*temp).title(), "Sheet1");
        xlnt_assert_equals((*iter).title(), "Sheet1");
        iter++;
        xlnt_assert_equals((*iter).title(), "Sheet2");
        temp = iter++;
        xlnt_assert_equals((*temp).title(), "Sheet2");
        xlnt_assert_equals(iter, wb2.end());

        iter = temp--;
        xlnt_assert_equals((*iter).title(), "Sheet2");
        xlnt_assert_equals((*temp).title(), "Sheet1");

        xlnt_assert_equals(xlnt::worksheet_iterator{}, xlnt::worksheet_iterator{});
    }

    void test_const_iter()
    {
        const xlnt::workbook wb;

        for (auto ws : wb)
        {
            xlnt_assert_equals(ws.title(), "Sheet1");
        }

        xlnt::workbook wb2;
        auto iter = wb.cbegin();
        auto iter_copy(iter); // copy ctor
        xlnt_assert_equals(iter, iter_copy);
        auto begin_2(wb2.cbegin());
        xlnt_assert_differs(begin_2, iter);
        iter = begin_2; // copy assign
        xlnt_assert_equals(iter, begin_2);
        iter = wb.cbegin();
        iter = std::move(begin_2);
        xlnt_assert_equals(iter, wb2.cbegin());
        // wb2 iterators assumed invalidated
        wb2.create_sheet();
        iter = wb2.cbegin();
        xlnt_assert_equals((*iter).title(), "Sheet1");
        ++iter;
        xlnt_assert_differs(iter, wb2.cbegin());
        xlnt_assert_equals((*iter).title(), "Sheet2");
        auto temp = --iter;
        xlnt_assert_equals((*temp).title(), "Sheet1");
        xlnt_assert_equals((*iter).title(), "Sheet1");
        iter++;
        xlnt_assert_equals((*iter).title(), "Sheet2");
        temp = iter++;
        xlnt_assert_equals((*temp).title(), "Sheet2");
        xlnt_assert_equals(iter, wb2.cend());

        iter = temp--;
        xlnt_assert_equals((*iter).title(), "Sheet2");
        xlnt_assert_equals((*temp).title(), "Sheet1");

        xlnt_assert_equals(xlnt::const_worksheet_iterator{}, xlnt::const_worksheet_iterator{});
    }

    void test_get_index()
    {
        xlnt::workbook wb;
        wb.create_sheet().title("1");
        wb.create_sheet().title("2");

        auto sheet_index = wb.index(wb.sheet_by_title("1"));
        xlnt_assert_equals(sheet_index, 1);

        sheet_index = wb.index(wb.sheet_by_title("2"));
        xlnt_assert_equals(sheet_index, 2);
    }

    void test_get_sheet_names()
    {
        xlnt::workbook wb;
        wb.create_sheet().title("test_get_sheet_titles");

        const std::vector<std::string> expected_titles = {"Sheet1", "test_get_sheet_titles"};

        xlnt_assert_equals(wb.sheet_titles(), expected_titles);
    }

    void test_add_named_range()
    {
        xlnt::workbook wb, wb2;
        auto new_sheet = wb.create_sheet();
        wb.create_named_range("test_nr", new_sheet, "A1");
        xlnt_assert(new_sheet.has_named_range("test_nr"));
        xlnt_assert(wb.has_named_range("test_nr"));
        xlnt_assert_throws(wb2.create_named_range("test_nr", new_sheet, "A1"), std::runtime_error);
    }

    void test_get_named_range()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        wb.create_named_range("test_nr", new_sheet, "A1");
        auto found_range = wb.named_range("test_nr");
        auto expected_range = new_sheet.range("A1");
        xlnt_assert_equals(expected_range, found_range);
        xlnt_assert_throws(wb.named_range("test_nr2"), std::runtime_error);
    }

    void test_remove_named_range()
    {
        xlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        wb.create_named_range("test_nr", new_sheet, "A1");
        wb.remove_named_range("test_nr");
        xlnt_assert(!new_sheet.has_named_range("test_nr"));
        xlnt_assert(!wb.has_named_range("test_nr"));
        xlnt_assert_throws(wb.remove_named_range("test_nr2"), std::runtime_error);
    }

    void test_post_increment_iterator()
    {
        xlnt::workbook wb;

        wb.create_sheet().title("Sheet2");
        wb.create_sheet().title("Sheet3");

        auto iter = wb.begin();

        xlnt_assert_equals((*(iter++)).title(), "Sheet1");
        xlnt_assert_equals((*(iter++)).title(), "Sheet2");
        xlnt_assert_equals((*(iter++)).title(), "Sheet3");
        xlnt_assert_equals(iter, wb.end());

        const auto wb_const = wb;

        auto const_iter = wb_const.begin();

        xlnt_assert_equals((*(const_iter++)).title(), "Sheet1");
        xlnt_assert_equals((*(const_iter++)).title(), "Sheet2");
        xlnt_assert_equals((*(const_iter++)).title(), "Sheet3");
        xlnt_assert_equals(const_iter, wb_const.end());
    }

    void test_clone()
    {
        xlnt::workbook wb1;
        wb1.create_sheet().title("NEW1");
        xlnt::workbook wb2_shallow_copy = wb1.clone(xlnt::workbook::clone_method::shallow_copy);
        wb1.sheet_by_title("NEW1").title("NEW_CHANGED");
        xlnt_assert_throws_nothing(wb2_shallow_copy.sheet_by_title("NEW_CHANGED"));
        xlnt::workbook wb3_deep_copy = wb2_shallow_copy.clone(xlnt::workbook::clone_method::deep_copy);
        wb3_deep_copy.sheet_by_title("NEW_CHANGED").title("NEW_CHANGED_AGAIN");
        xlnt_assert_throws(wb2_shallow_copy.sheet_by_title("NEW_CHANGED_AGAIN"), xlnt::key_not_found);
        xlnt_assert_throws(wb1.sheet_by_title("NEW_CHANGED_AGAIN"), xlnt::key_not_found);
    }

    void test_copy_constructor()
    {
        xlnt::workbook wb1;
        wb1.create_sheet().title("NEW1");
        xlnt::workbook wb2_copy(wb1);
        wb1.sheet_by_title("NEW1").title("NEW_CHANGED");
        // Ensure shallow copy.
        xlnt_assert_throws_nothing(wb2_copy.sheet_by_title("NEW_CHANGED"));
    }

    void test_copy_assignment_operator()
    {
        xlnt::workbook wb1;
        wb1.create_sheet().title("NEW1");
        xlnt::workbook wb2_copy;
        wb2_copy = wb1;
        wb1.sheet_by_title("NEW1").title("NEW_CHANGED");
        // Ensure shallow copy.
        xlnt_assert_throws_nothing(wb2_copy.sheet_by_title("NEW_CHANGED"));
    }

    void test_copy_iterator()
    {
        xlnt::workbook wb;

        wb.create_sheet().title("Sheet2");
        wb.create_sheet().title("Sheet3");

        auto iter = wb.begin();
        xlnt_assert_equals((*iter).title(), "Sheet1");

        iter++;
        xlnt_assert_equals((*iter).title(), "Sheet2");

        auto copy = wb.begin();
        copy = iter;
        xlnt_assert_equals((*iter).title(), "Sheet2");
        xlnt_assert_equals(iter, copy);

        iter++;
        xlnt_assert_equals((*iter).title(), "Sheet3");
        xlnt_assert_differs(iter, copy);

        copy++;
        xlnt_assert_equals((*iter).title(), "Sheet3");
        xlnt_assert_equals(iter, copy);
    }

    void test_manifest()
    {
        xlnt::manifest m;
        xlnt_assert(!m.has_default_type("xml"));
        xlnt_assert_throws(m.default_type("xml"), xlnt::key_not_found);
        xlnt_assert(!m.has_relationship(xlnt::path("/"), xlnt::relationship_type::office_document));
        xlnt_assert(m.relationships(xlnt::path("xl/workbook.xml")).empty());
    }

    void test_memory()
    {
        xlnt::workbook wb, wb2;
        wb.active_sheet().title("swap");
        std::swap(wb, wb2);
        xlnt_assert_equals(wb.active_sheet().title(), "Sheet1");
        xlnt_assert_equals(wb2.active_sheet().title(), "swap");
        wb = wb2;
        xlnt_assert_equals(wb.active_sheet().title(), "swap");
    }

    void test_clear()
    {
        xlnt::workbook wb;
        xlnt::style s = wb.create_style("s");
        wb.active_sheet().cell("B2").value("B2");
        wb.active_sheet().cell("B2").style(s);
        xlnt_assert(wb.active_sheet().cell("B2").has_style());
        wb.clear_styles();
        xlnt_assert(!wb.active_sheet().cell("B2").has_style());
        xlnt::format format = wb.create_format();
        xlnt::font font;
        font.size(41);
        format.font(font, true);
        wb.active_sheet().cell("B2").format(format);
        xlnt_assert(wb.active_sheet().cell("B2").has_format());
        wb.clear_formats();
        xlnt_assert(!wb.active_sheet().cell("B2").has_format());
        wb.clear();
        xlnt_assert(wb.sheet_titles().empty());
    }

    void test_comparison()
    {
        xlnt::workbook wb, wb2;
        xlnt_assert(wb == wb);
        xlnt_assert(wb.compare(wb, true));
        xlnt_assert(wb.compare(wb, false));
        xlnt_assert(!(wb != wb));
        xlnt_assert(!(wb == wb2));
        xlnt_assert(wb != wb2);
        xlnt_assert(!wb.compare(wb2, true));
        xlnt_assert(wb.compare(wb2, false));

        const auto &wb_const = wb;
        //TODO these aren't tests...
        wb_const.manifest();

        xlnt_assert(wb.has_theme());

        wb.create_style("style1");
        wb.style("style1");
        wb_const.style("style1");
    }

    void test_id_gen()
    {
        xlnt::workbook wb;
        wb.create_sheet();
        wb.create_sheet();
        wb.remove_sheet(wb[1]);
        wb.create_sheet();
        xlnt_assert_differs(wb[1].id(), wb[2].id());
    }

    void test_load_file()
    {
        xlnt::path file = path_helper::test_file("2_minimal.xlsx");
        xlnt::workbook wb_path(file);
        // ctor from ifstream
        std::ifstream file_reader(file.string(), std::ios::binary);
        xlnt_assert(wb_path.compare(xlnt::workbook(file_reader), false));
        // load with string
        xlnt::workbook wb_load1;
        xlnt_assert_differs(wb_path, wb_load1);
        wb_load1.load(file.string());
        xlnt_assert(wb_path.compare(wb_load1, false));
        // load with wstring
        xlnt::workbook wb_load2;
        wb_load2.load(file.string());
        xlnt_assert(wb_path.compare(wb_load2, false));
        // load with path
        xlnt::workbook wb_load3;
        wb_load3.load(file);
        xlnt_assert(wb_path.compare(wb_load3, false));
        // load with istream
        xlnt::workbook wb_load4;
        std::ifstream file_reader2(file.string(), std::ios::binary);
        wb_load4.load(file_reader2);
        xlnt_assert(wb_path.compare(wb_load4, false));
        // load with vector
        std::ifstream file_reader3(file.string(), std::ios::binary);
        file_reader3.unsetf(std::ios::skipws);
        std::vector<uint8_t> data(std::istream_iterator<uint8_t>{file_reader3},
            std::istream_iterator<uint8_t>());
        xlnt::workbook wb_load5;
        wb_load5.load(data);
        xlnt_assert(wb_path.compare(wb_load5, false));
    }

    void test_load_file_encrypted()
    {
        const auto password = u8"\u043F\u0430\u0440\u043E\u043B\u044C"; // u8"пароль"
        xlnt::path file = path_helper::test_file("6_encrypted_libre.xlsx");
#ifdef __cpp_lib_char8_t
        // The reinterpret_cast is ugly as hell, but I'm not sure if duplicating
        // the path_helper and xlnt::path functionality is worth it...
        std::u8string file_as_string = reinterpret_cast<const char8_t *>(file.string().c_str());
#else
        std::string file_as_string = file.string();
#endif
        xlnt::workbook wb_path(file, password);
        // ctor from ifstream
        std::ifstream file_reader(file.string(), std::ios::binary);
        xlnt_assert(wb_path.compare(xlnt::workbook(file_reader, password), false));
        // load with string
        xlnt::workbook wb_load1;
        xlnt_assert_differs(wb_path, wb_load1);
        wb_load1.load(file_as_string, password);
        xlnt_assert(wb_path.compare(wb_load1, false));
        // load with wstring
        xlnt::workbook wb_load2;
        wb_load2.load(file_as_string, password);
        xlnt_assert(wb_path.compare(wb_load2, false));
        // load with path
        xlnt::workbook wb_load3;
        wb_load3.load(file, password);
        xlnt_assert(wb_path.compare(wb_load3, false));
        // load with istream
        xlnt::workbook wb_load4;
        std::ifstream file_reader2(file.string(), std::ios::binary);
        wb_load4.load(file_reader2, password);
        xlnt_assert(wb_path.compare(wb_load4, false));
        // load with vector
        std::ifstream file_reader3(file.string(), std::ios::binary);
        file_reader3.unsetf(std::ios::skipws);
        std::vector<uint8_t> data(std::istream_iterator<uint8_t>{file_reader3},
            std::istream_iterator<uint8_t>());
        xlnt::workbook wb_load5;
        wb_load5.load(data, password);
        xlnt_assert(wb_path.compare(wb_load5, false));
    }

    void test_Issue279()
    {
        xlnt::workbook wb(path_helper::test_file("Issue279_workbook_delete_rename.xlsx"));
        while (wb.sheet_count() > 1)
        {
            if (wb[1].title() != "BOM")
            {
                wb.remove_sheet(wb[1]);
            }
            else
            {
                wb.remove_sheet(wb[0]);
            }
        }
        // get sheet bom change title
        auto ws1 = wb.sheet_by_index(0);
        ws1.title("checkedBom");
        // report sheet
        auto ws2 = wb.create_sheet(1);
        ws2.title("REPORT");
        //save a copy file
        wb.save("temp.xlsx");
    }

    void test_Issue353()
    {
        xlnt::workbook wb1;
        wb1.load(path_helper::test_file("Issue353_first_row_empty_w_properties.xlsx"));
        wb1.save("temp_issue353.xlsx");

        xlnt::workbook wb2;
        wb2.load("temp_issue353.xlsx");
        auto ws = wb2.active_sheet();
        xlnt_assert_equals(ws.row_properties(1).spans.get(), "1:8");
        xlnt_assert_equals(ws.row_properties(17).spans.get(), "2:7");
    }

    void test_Issue494()
    {
        xlnt::workbook wb;
        wb.load(path_helper::test_file("Issue494_shared_string.xlsx"));
        auto ws = wb.active_sheet();
        xlnt_assert_equals(ws.cell(2, 1).to_string(), "V1.00");
        xlnt_assert_equals(ws.cell(2, 2).to_string(), "V1.00");
    }

    void test_Issue90()
    {
        // Test for Issue #90: Parsing Exception: bad cell coordinates for definedName with Whole-Column Reference
        // This test verifies that workbooks with definedName entries containing whole-column references
        // (e.g., $A:$I) can be loaded without throwing exceptions.

        xlnt::workbook wb;

        // This should not throw an exception
        xlnt_assert_throws_nothing(wb.load(path_helper::test_file("issue90_debug_test_file.xlsx")));

        // Verify that the workbook was loaded successfully
        xlnt_assert(wb.sheet_count() > 0);

        // Test that we can access the worksheets
        auto ws = wb.active_sheet();
        xlnt_assert_throws_nothing(ws.title());

        // Test direct range_reference parsing with whole column references
        xlnt_assert_throws_nothing(xlnt::range_reference("A:I"));
        xlnt_assert_throws_nothing(xlnt::range_reference("$A:$I"));
        xlnt_assert_throws_nothing(xlnt::range_reference("B:E"));
        xlnt_assert_throws_nothing(xlnt::range_reference("$B:$E"));

        // Test direct range_reference parsing with whole row references
        xlnt_assert_throws_nothing(xlnt::range_reference("1:5"));
        xlnt_assert_throws_nothing(xlnt::range_reference("$1:$5"));
        xlnt_assert_throws_nothing(xlnt::range_reference("10:20"));
        xlnt_assert_throws_nothing(xlnt::range_reference("$10:$20"));

        // Verify that the parsed ranges have correct properties
        xlnt::range_reference col_ref("A:C");
        xlnt_assert_equals(col_ref.top_left().column(), xlnt::column_t("A"));
        xlnt_assert_equals(col_ref.top_left().row(), 1);
        xlnt_assert_equals(col_ref.bottom_right().column(), xlnt::column_t("C"));
        xlnt_assert_equals(col_ref.bottom_right().row(), xlnt::constants::max_row());

        xlnt::range_reference row_ref("2:4");
        xlnt_assert_equals(row_ref.top_left().column(), xlnt::constants::min_column());
        xlnt_assert_equals(row_ref.top_left().row(), 2);
        xlnt_assert_equals(row_ref.bottom_right().column(), xlnt::constants::max_column());
        xlnt_assert_equals(row_ref.bottom_right().row(), 4);
    }

    void test_style()
    {
        xlnt::workbook wb;
        xlnt_assert_equals(wb.has_style("my_custom_style"), false);
        wb.create_style("my_custom_style");
        xlnt_assert_equals(wb.has_style("my_custom_style"), true);
    }

    void test_sheet_moving()
    {
        xlnt::workbook testWorkbook;
        testWorkbook.create_sheet();
        testWorkbook.create_sheet();
        testWorkbook.create_sheet();
        testWorkbook.create_sheet();
        testWorkbook.create_sheet();

        auto movingSheet = testWorkbook.sheet_by_index(3);

        // move sheet to the beginning
        size_t newIndex = 0;
        xlnt_assert_throws_nothing(testWorkbook.move_sheet(movingSheet, newIndex));
        xlnt_assert_equals(testWorkbook.index(movingSheet), newIndex);

        // move sheet to the middle
        newIndex = 2;
        xlnt_assert_throws_nothing(testWorkbook.move_sheet(movingSheet, newIndex));
        xlnt_assert_equals(testWorkbook.index(movingSheet), newIndex);

        // move sheet to the end
        newIndex = testWorkbook.sheet_count() - 1;
        xlnt_assert_throws_nothing(testWorkbook.move_sheet(movingSheet, newIndex));
        xlnt_assert_equals(testWorkbook.index(movingSheet), newIndex);

        // try to move to incorrect position
        newIndex = testWorkbook.sheet_count() + 100;
        xlnt_assert_throws(testWorkbook.move_sheet(movingSheet, newIndex), xlnt::invalid_parameter);

        // try to move a sheet belonging to another workbook inside test workbook
        newIndex = 2;
        xlnt::workbook externalWorkbook;
        auto exterbalSheet = externalWorkbook.sheet_by_index(0);
        xlnt_assert_throws(testWorkbook.move_sheet(exterbalSheet, newIndex), xlnt::invalid_parameter);
    }
};
static workbook_test_suite x;
