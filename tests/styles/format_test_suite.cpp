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

#include <xlnt/styles/format.hpp>
#include <helpers/test_suite.hpp>
#include <xlnt/xlnt.hpp>

class format_test_suite : public test_suite
{
public:
    format_test_suite()
    {
        register_test(test_clone);
        register_test(test_compare);
    }

    void test_clone()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format("test_format");
        format.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::red())));
        xlnt::format format_simple_copy = format;
        format.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::green())));
        xlnt_assert_equals(format_simple_copy.border().side(xlnt::border_side::bottom).get().color().get(), xlnt::color::green());
        xlnt::format format_shallow_copy = format.clone(xlnt::clone_method::shallow_copy);
        format.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::blue())));
        xlnt_assert_equals(format_shallow_copy.border().side(xlnt::border_side::bottom).get().color().get(), xlnt::color::blue());
        xlnt::format format_deep_copy = format.clone(xlnt::clone_method::deep_copy);
        format.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::darkred())));
        xlnt_assert_equals(format_deep_copy.border().side(xlnt::border_side::bottom).get().color().get(), xlnt::color::blue());
    }

    void test_compare()
    {
        xlnt::workbook wb;
        xlnt::format format = wb.create_format("test_format");
        xlnt::format format_simple_copy = format;
        xlnt_assert_equals(format, format_simple_copy);
        xlnt_assert(format.compare(format_simple_copy, true));
        xlnt_assert(format.compare(format_simple_copy, false));
        xlnt::format format_shallow_copy = format.clone(xlnt::clone_method::shallow_copy);
        xlnt_assert_equals(format, format_shallow_copy);
        xlnt_assert(format.compare(format_shallow_copy, true));
        xlnt_assert(format.compare(format_shallow_copy, false));
        xlnt::format format_deep_copy = format.clone(xlnt::clone_method::deep_copy);
        xlnt_assert_differs(format, format_deep_copy);
        xlnt_assert(!format.compare(format_deep_copy, true));
        xlnt_assert(format.compare(format_deep_copy, false));
    }
};

static format_test_suite x;
