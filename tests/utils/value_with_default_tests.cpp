// Copyright (c) 2026 xlnt-community
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

#include <xlnt/utils/numeric.hpp>
#include <xlnt/utils/value_with_default.h>

#include <helpers/test_suite.hpp>

class value_with_default_test_suite : public test_suite
{
public:
    value_with_default_test_suite()
        : test_suite()
    {
        register_test(test_ctor);
        register_test(test_copy_ctor);
        register_test(test_move_ctor);
        register_test(test_copy_assign);
        register_test(test_move_assign);
        register_test(test_set_and_get);
        register_test(test_equality);
        register_test(test_const);
        register_test(test_different_default);
        register_test(test_double);
    }

    void test_ctor()
    {
        const int default_value = 100;
        // default
        xlnt::detail::value_with_default<int, default_value> opt1;
        xlnt_assert(!opt1.is_set());
        xlnt_assert(opt1.is_default());
        xlnt_assert_equals(opt1, default_value);
        // value
        const int test_val = 3;
        xlnt::detail::value_with_default<int, default_value> opt2(test_val);
        xlnt_assert(opt2.is_set());
        xlnt_assert(!opt2.is_default());
        xlnt_assert_equals(opt2.get(), test_val);
    }

    void test_copy_ctor()
    {
        const int default_value = 100;
        xlnt::detail::value_with_default<int, default_value> opt1;
        xlnt::detail::value_with_default<int, default_value> opt2(opt1);
        xlnt_assert_equals(opt1, opt2);

        const int test_val = 123;
        xlnt::detail::value_with_default<int, default_value> opt3(test_val);
        xlnt::detail::value_with_default<int, default_value> opt4(opt3);
        xlnt_assert_equals(opt3, opt4);
    }

    void test_move_ctor()
    {
        const int default_value = 100;
        xlnt::detail::value_with_default<int, default_value> opt1;
        xlnt::detail::value_with_default<int, default_value> opt2(std::move(opt1));
        xlnt::detail::value_with_default<int, default_value> opt1b;
        xlnt_assert_equals(opt2, opt1b); // can't test against opt1 so use a temporary

        const int test_val = 123;
        xlnt::detail::value_with_default<int, default_value> opt3(test_val);
        xlnt::detail::value_with_default<int, default_value> opt4(std::move(opt3));
        xlnt_assert(opt4.is_set()); // moved to value_with_default contains the value
        xlnt_assert_equals(opt4.get(), test_val);
    }

    void test_copy_assign()
    {
        const int default_value = 100;
        xlnt::detail::value_with_default<int, default_value> opt1;
        xlnt::detail::value_with_default<int, default_value> opt_assign1; // to actually test assignment, the value needs to be already created. using '=' is not enough
        opt_assign1 = opt1;
        xlnt_assert_equals(opt1, opt_assign1);

        const int test_val = 123;
        xlnt::detail::value_with_default<int, default_value> opt2(test_val);
        xlnt::detail::value_with_default<int, default_value> opt_assign2;
        opt_assign2 = opt2;
        xlnt_assert_equals(opt2, opt_assign2);
    }

    void test_move_assign()
    {
        const int default_value = 100;
        xlnt::detail::value_with_default<int, default_value> opt1;
        xlnt::detail::value_with_default<int, default_value> opt_assign1; // to actually test assignment, the value needs to be already created. using '=' is not enough
        xlnt::detail::value_with_default<int, default_value> opt1b;
        opt_assign1 = std::move(opt1);
        xlnt_assert_equals(opt_assign1, opt1b); // can't test against opt1 so use a temporary

        const int test_val = 123;
        xlnt::detail::value_with_default<int, default_value> opt2(test_val);
        xlnt::detail::value_with_default<int, default_value> opt_assign2;
        opt_assign2 = std::move(opt2);
        xlnt_assert(opt_assign2.is_set()); // moved to value_with_default contains the value
        xlnt_assert_equals(opt_assign2.get(), test_val);
    }

    void test_set_and_get()
    {
        const int default_value = 100;
        xlnt::detail::value_with_default<int, default_value> test_opt;
        xlnt_assert(!test_opt.is_set());
        // set
        const int test_val1 = 321;
        test_opt = test_val1;
        xlnt_assert(test_opt.is_set());
        xlnt_assert_equals(test_opt.get(), test_val1);
        // set again
        const int test_val2 = 123;
        test_opt = test_val2;
        xlnt_assert(test_opt.is_set());
        xlnt_assert_equals(test_opt.get(), test_val2);
        // operator= set
        xlnt::detail::value_with_default<int, default_value> test_opt2;
        test_opt2 = test_val1;
        xlnt_assert_equals(test_opt2.get(), test_val1);
    }

    void test_equality()
    {
        const int default_value = 100;

        xlnt::detail::value_with_default<int, default_value> test_opt1;
        xlnt::detail::value_with_default<int, default_value> test_opt2;
        // no value opts compare equal
        xlnt_assert(test_opt1 == test_opt2);
        xlnt_assert(!(test_opt1 != test_opt2));
        xlnt_assert(test_opt2 == test_opt1);
        xlnt_assert(!(test_opt2 != test_opt1));
        // value compares false with no value
        const int test_val = 1;
        test_opt1 = test_val;
        xlnt_assert(test_opt1 != test_opt2);
        xlnt_assert(!(test_opt1 == test_opt2));
        xlnt_assert(test_opt2 != test_opt1);
        xlnt_assert(!(test_opt2 == test_opt1));
        // value compares false with a different value
        const int test_val2 = 2;
        test_opt2 = test_val2;
        xlnt_assert(test_opt1 != test_opt2);
        xlnt_assert(!(test_opt1 == test_opt2));
        xlnt_assert(test_opt2 != test_opt1);
        xlnt_assert(!(test_opt2 == test_opt1));
        // value compares equal with same value
        test_opt2 = test_val;
        xlnt_assert(test_opt1 == test_opt2);
        xlnt_assert(!(test_opt1 != test_opt2));
        xlnt_assert(test_opt2 == test_opt1);
        xlnt_assert(!(test_opt2 != test_opt1));
    }

    void test_const()
    {
        const int default_value = 100;

        // functions on a const value_with_default
        const int test_val = 1;
        const xlnt::detail::value_with_default<int, default_value> opt(test_val);
        xlnt_assert(opt.is_set());
        xlnt_assert(opt.get() == test_val);

        xlnt::detail::value_with_default<int, default_value> opt2(test_val);
        xlnt_assert(opt == opt2);
        xlnt_assert(opt2 == opt);
        xlnt_assert(!(opt != opt2));
        xlnt_assert(!(opt2 != opt));
    }

    void test_different_default()
    {
        const int default_value1 = 100;
        const int default_value2 = 123;
        xlnt::detail::value_with_default<int, default_value1> opt1;
        xlnt::detail::value_with_default<int, default_value2> opt2;
        xlnt_assert_differs(opt1, opt2);
        xlnt_assert(opt1 != opt2);
    }

    void test_double()
    {
        xlnt::detail::double_with_default<1, 1000> v1;
        xlnt_assert(xlnt::detail::float_equals(v1, 0.001));

        xlnt::detail::double_with_default<100> v2;
        xlnt_assert(xlnt::detail::float_equals(v2.get(), 100.0));

        xlnt::detail::fp_with_default_nan<double> v3;
        xlnt_assert(v3.is_default());
    }
};
static value_with_default_test_suite x;
