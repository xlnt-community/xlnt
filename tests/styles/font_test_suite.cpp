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

#include <xlnt/styles/font.hpp>

class font_test_suite : public test_suite
{
public:
    font_test_suite()
    {
        register_test(test_color);
        register_test(test_family);
        register_test(test_charset);
        register_test(test_scheme);
    }

    void test_color()
    {
        xlnt::font font;
        xlnt_assert(!font.has_color());
        xlnt_assert_throws(font.color(), xlnt::invalid_attribute);

        xlnt::color color;
        font.color(color);

        xlnt_assert(font.has_color());
        xlnt_assert_equals(font.color(), color);
    }

    void test_family()
    {
        xlnt::font font;
        xlnt_assert(!font.has_family());
        xlnt_assert_throws(font.family(), xlnt::invalid_attribute);

        font.family(0);

        xlnt_assert(font.has_family());
        xlnt_assert_equals(font.family(), 0);
    }

    void test_charset()
    {
        xlnt::font font;
        xlnt_assert(!font.has_charset());
        xlnt_assert_throws(font.charset(), xlnt::invalid_attribute);

        font.charset(0);

        xlnt_assert(font.has_charset());
        xlnt_assert_equals(font.charset(), 0);
    }

    void test_scheme()
    {
        xlnt::font font;
        xlnt_assert(!font.has_scheme());
        xlnt_assert_throws(font.scheme(), xlnt::invalid_attribute);

        font.scheme("scheme");

        xlnt_assert(font.has_scheme());
        xlnt_assert_equals(font.scheme(), "scheme");
    }
};
static font_test_suite x;
