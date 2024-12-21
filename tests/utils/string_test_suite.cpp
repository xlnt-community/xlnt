// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2024 xlnt-community
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

#include <detail/utils/string_helpers.hpp>
#include <helpers/test_suite.hpp>

class string_test_suite : public test_suite
{
public:
    string_test_suite()
    {
        register_test(test_split_string_space);
        register_test(test_split_string_comma);
        register_test(test_split_string_single);
        register_test(test_split_string_empty);
        register_test(test_join_space);
        register_test(test_join_comma);
    }

    void test_split_string_space()
    {
        auto s = xlnt::detail::split_string("a bc1,def19 ", ',');
        xlnt_assert_equals(s.size(), 2);
        xlnt_assert_equals(s[0], "a bc1");
        xlnt_assert_equals(s[1], "def19 ");
        xlnt_assert_differs(s[1], "def19");
    }

    void test_split_string_comma()
    {
        auto s = xlnt::detail::split_string("a bc1 def19", ' ');
        xlnt_assert_equals(s.size(), 3);
        xlnt_assert_equals(s[0], "a");
        xlnt_assert_equals(s[1], "bc1");
        xlnt_assert_equals(s[2], "def19");
    }

    void test_split_string_single()
    {
        auto s = xlnt::detail::split_string("a", ' ');
        xlnt_assert_equals(s.size(), 1);
        xlnt_assert_equals(s[0], "a");
    }

    void test_split_string_empty()
    {
        auto s = xlnt::detail::split_string("", ' ');
        xlnt_assert_equals(s.size(), 0);
    }

    struct X
    {
        const std::string& to_string() const {return s;}
        std::string s;
    };

    void test_join_space()
    {
        std::vector<X> items = {X{"A"}, X{"B"}, X{"C1"}};

        xlnt_assert_equals(xlnt::detail::join(items, ' '), "A B C1");
    }

    void test_join_comma()
    {
        std::vector<X> items = {X{"A"}, X{"B"}, X{"C1"}};

        xlnt_assert_equals(xlnt::detail::join(items, ','), "A,B,C1");
    }
};
static string_test_suite x;
