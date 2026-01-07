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
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/cell_vector.hpp>
#include <xlnt/worksheet/range.hpp>

class cell_vector_test_suite : public test_suite
{
public:
    cell_vector_test_suite()
    {
        register_test(test_mutable_vector_index);
        register_test(test_const_vector_index);
    }

    void test_mutable_vector_index()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.cell("A1").value("A1");
        xlnt::range range = ws.range("A1:B1");
        xlnt::cell_vector vec = range.front();
        xlnt_assert_equals(vec.length(), 2);
        xlnt_assert(ws.has_cell("A1"));
        xlnt_assert_equals(vec[0].value<std::string>(), "A1");
        xlnt_assert(!ws.has_cell("B1"));
        // Accessing the element will create the cell.
        xlnt_assert(!vec[1].has_value());
        xlnt_assert(ws.has_cell("B1"));
    }

    void test_const_vector_index()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        ws.cell("A1").value("A1");
        xlnt::range range = ws.range("A1:B1");
        const xlnt::cell_vector vec = range.front();
        xlnt_assert_equals(vec.length(), 2);
        xlnt_assert(ws.has_cell("A1"));
        xlnt_assert_equals(vec[0].value<std::string>(), "A1");
        xlnt_assert(!ws.has_cell("B1"));
        xlnt_assert_throws(vec[1].has_value(), xlnt::invalid_parameter);
    }
};
static cell_vector_test_suite x;
