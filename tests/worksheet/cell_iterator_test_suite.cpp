// Copyright (c) 2025-2026 xlnt-community
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
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/cell_vector.hpp>
#include <xlnt/worksheet/range.hpp>

class cell_iterator_test_suite : public test_suite
{
public:
    cell_iterator_test_suite()
    {
        register_test(test_mutable_iterator);
        register_test(test_const_iterator);
    }

    void test_mutable_iterator()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.cell("A1").value("A1");
        xlnt::range range = ws.range("A1:B1");
        xlnt::cell_vector vec = range.front();
        xlnt_assert_equals(vec.length(), 2);
        auto it = vec.begin();
        xlnt_assert(ws.has_cell("A1"));
        xlnt_assert(it.has_value());
        xlnt_assert_equals((*it).value<std::string>(), "A1");
        ++it;
        xlnt_assert(!ws.has_cell("B1"));
        // Accessing the non-existing cell B1 will create the cell.
        xlnt_assert(!(*it).has_value());
        xlnt_assert(ws.has_cell("B1"));
    }

    void test_const_iterator()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.cell("A1").value("A1");
        xlnt::range range = ws.range("A1:B1");
        xlnt::cell_vector vec = range.front();
        xlnt_assert_equals(vec.length(), 2);
        auto it = vec.cbegin();
        xlnt_assert(ws.has_cell("A1"));
        xlnt_assert(it.has_value());
        xlnt_assert_equals((*it).value<std::string>(), "A1");
        ++it;
        xlnt_assert(!ws.has_cell("B1"));
        xlnt_assert(!it.has_value());
        xlnt_assert_throws(*it, xlnt::invalid_parameter);
        xlnt_assert(!ws.has_cell("B1"));
    }
};
static cell_iterator_test_suite x;
