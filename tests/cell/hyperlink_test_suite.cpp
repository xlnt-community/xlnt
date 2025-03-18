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

#include <xlnt/cell/hyperlink.hpp>
#include <helpers/test_suite.hpp>
#include <xlnt/xlnt.hpp>

class hyperlink_test_suite : public test_suite
{
public:
    hyperlink_test_suite()
    {
        register_test(test_clone);
        register_test(test_compare);
    }

    void test_clone()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        xlnt::cell cell11 = ws.cell(1, 1);
        cell11.hyperlink("https://www.example.com");
        xlnt::hyperlink hyperlink = cell11.hyperlink();
        hyperlink.tooltip("https://www.example.com");
        xlnt::hyperlink hyperlink_simple_copy = hyperlink;
        hyperlink.tooltip("https://www.example.org");
        xlnt_assert_equals(hyperlink_simple_copy.tooltip(), "https://www.example.org");
        xlnt::hyperlink hyperlink_shallow_copy = hyperlink.clone(xlnt::clone_method::shallow_copy);
        hyperlink.tooltip("https://www.example.net");
        xlnt_assert_equals(hyperlink_shallow_copy.tooltip(), "https://www.example.net");
        xlnt::hyperlink hyperlink_deep_copy = hyperlink.clone(xlnt::clone_method::deep_copy);
        hyperlink.tooltip("https://www.example");
        xlnt_assert_equals(hyperlink_deep_copy.tooltip(), "https://www.example.net");
    }

    void test_compare()
    {
        xlnt::workbook wb;
        xlnt::worksheet ws = wb.active_sheet();
        xlnt::cell cell11 = ws.cell(1, 1);
        cell11.hyperlink("https://www.example.com");
        xlnt::hyperlink hyperlink = cell11.hyperlink();
        xlnt::hyperlink hyperlink_simple_copy = hyperlink;
        xlnt_assert_equals(hyperlink, hyperlink_simple_copy);
        xlnt_assert(hyperlink.compare(hyperlink_simple_copy, true));
        xlnt_assert(hyperlink.compare(hyperlink_simple_copy, false));
        xlnt::hyperlink hyperlink_shallow_copy = hyperlink.clone(xlnt::clone_method::shallow_copy);
        xlnt_assert_equals(hyperlink, hyperlink_shallow_copy);
        xlnt_assert(hyperlink.compare(hyperlink_shallow_copy, true));
        xlnt_assert(hyperlink.compare(hyperlink_shallow_copy, false));
        xlnt::hyperlink hyperlink_deep_copy = hyperlink.clone(xlnt::clone_method::deep_copy);
        xlnt_assert_differs(hyperlink, hyperlink_deep_copy);
        xlnt_assert(!hyperlink.compare(hyperlink_deep_copy, true));
        xlnt_assert(hyperlink.compare(hyperlink_deep_copy, false));
    }
};

static hyperlink_test_suite x;
