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

#include <xlnt/styles/style.hpp>
#include <xlnt/xlnt.hpp>
#include <helpers/test_suite.hpp>

class style_test_suite : public test_suite
{
public:
    style_test_suite()
    {
        register_test(test_all);
        register_test(test_clone);
        register_test(test_compare);
    }

    void test_all()
    {
        xlnt::workbook wb;
        auto test_style = wb.create_style("test_style");
        test_style.number_format(xlnt::number_format::date_ddmmyyyy());

        auto copy_style(test_style);
        xlnt_assert_equals(test_style, copy_style);

        // number format
        xlnt_assert_equals(copy_style.name(), "test_style");
        xlnt_assert_equals(copy_style.number_format(), xlnt::number_format::date_ddmmyyyy());
        //xlnt_assert(!copy_style.number_format_applied()); // this doesn't seem to have sensible behaviour?
        copy_style.number_format(xlnt::number_format::date_datetime(), true); // true applied param
        xlnt_assert_equals(copy_style.number_format(), xlnt::number_format::date_datetime());
        xlnt_assert(copy_style.number_format_applied());
        copy_style.number_format(xlnt::number_format::date_dmminus(), false); // false applied param
        xlnt_assert_equals(copy_style.number_format(), xlnt::number_format::date_dmminus());
        xlnt_assert(!copy_style.number_format_applied());

        xlnt_assert(!copy_style.pivot_button());
        copy_style.pivot_button(true);
        xlnt_assert(copy_style.pivot_button());

        xlnt_assert(!copy_style.quote_prefix());
        copy_style.quote_prefix(true);
        xlnt_assert(copy_style.quote_prefix());
    }

    void test_clone()
    {
        xlnt::workbook wb;
        xlnt::style style = wb.create_style("test_style");
        style.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::red())));
        xlnt::style style_simple_copy = style;
        style.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::green())));
        xlnt_assert_equals(style_simple_copy.border().side(xlnt::border_side::bottom).get().color().get(), xlnt::color::green());
        xlnt::style style_shallow_copy = style.clone(xlnt::clone_method::shallow_copy);
        style.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::blue())));
        xlnt_assert_equals(style_shallow_copy.border().side(xlnt::border_side::bottom).get().color().get(), xlnt::color::blue());
        xlnt::style style_deep_copy = style.clone(xlnt::clone_method::deep_copy);
        style.border(xlnt::border().side(xlnt::border_side::bottom, xlnt::border::border_property().color(xlnt::color::darkred())));
        xlnt_assert_equals(style_deep_copy.border().side(xlnt::border_side::bottom).get().color().get(), xlnt::color::blue());
    }

    void test_compare()
    {
        xlnt::workbook wb;
        xlnt::style style = wb.create_style("test_style");
        xlnt::style style_simple_copy = style;
        xlnt_assert_equals(style, style_simple_copy);
        xlnt_assert(style.compare(style_simple_copy, true));
        xlnt_assert(style.compare(style_simple_copy, false));
        xlnt::style style_shallow_copy = style.clone(xlnt::clone_method::shallow_copy);
        xlnt_assert_equals(style, style_shallow_copy);
        xlnt_assert(style.compare(style_shallow_copy, true));
        xlnt_assert(style.compare(style_shallow_copy, false));
        xlnt::style style_deep_copy = style.clone(xlnt::clone_method::deep_copy);
        xlnt_assert_differs(style, style_deep_copy);
        xlnt_assert(!style.compare(style_deep_copy, true));
        xlnt_assert(style.compare(style_deep_copy, false));
    }
};
static style_test_suite x;
