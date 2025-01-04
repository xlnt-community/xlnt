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

#include <detail/parsers.hpp>
#include <detail/locale.hpp>

#include <helpers/test_suite.hpp>

class parser_test_suite : public test_suite
{
public:
    parser_test_suite()
    {
        register_test(test_parse_double_with_dot);
        register_test(test_parse_double_with_comma);
        register_test(test_parse_double_large);
        register_test(test_parse_double_with_classic_locale);
        register_test(test_parse_double_with_system_locale);
        register_test(test_parse_double_out_of_range);
        register_test(test_parse_double_empty);
        register_test(test_parse_double_text);
        register_test(test_parse_float_with_dot);
        register_test(test_parse_float_with_comma);
        register_test(test_parse_float_large);
        register_test(test_parse_float_with_classic_locale);
        register_test(test_parse_float_with_system_locale);
        register_test(test_parse_float_out_of_range);
        register_test(test_parse_float_empty);
        register_test(test_parse_float_text);
        register_test(test_parse_long_double_with_dot);
        register_test(test_parse_long_double_with_comma);
        register_test(test_parse_long_double_large);
        register_test(test_parse_long_double_with_classic_locale);
        register_test(test_parse_long_double_with_system_locale);
        register_test(test_parse_long_double_out_of_range);
        register_test(test_parse_long_double_empty);
        register_test(test_parse_long_double_text);
        register_test(test_parse_int);
        register_test(test_parse_int_out_of_range);
        register_test(test_parse_int_empty);
        register_test(test_parse_int_text);
        register_test(test_parse_long);
        register_test(test_parse_long_out_of_range);
        register_test(test_parse_long_empty);
        register_test(test_parse_long_text);
        register_test(test_parse_long_long);
        register_test(test_parse_long_long_out_of_range);
        register_test(test_parse_long_long_empty);
        register_test(test_parse_long_long_text);
        register_test(test_parse_unsigned_int);
        register_test(test_parse_unsigned_int_out_of_range);
        register_test(test_parse_unsigned_int_empty);
        register_test(test_parse_unsigned_int_text);
        register_test(test_parse_unsigned_int_minus_string);
        register_test(test_parse_unsigned_int_minus_number);
        register_test(test_parse_unsigned_long);
        register_test(test_parse_unsigned_long_out_of_range);
        register_test(test_parse_unsigned_long_empty);
        register_test(test_parse_unsigned_long_text);
        register_test(test_parse_unsigned_long_minus_string);
        register_test(test_parse_unsigned_long_minus_number);
        register_test(test_parse_unsigned_long_long);
        register_test(test_parse_unsigned_long_long_out_of_range);
        register_test(test_parse_unsigned_long_long_empty);
        register_test(test_parse_unsigned_long_long_text);
        register_test(test_parse_unsigned_long_long_minus_string);
        register_test(test_parse_unsigned_long_long_minus_number);
    }

    void test_parse_double_with_dot()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse("2.3", result, nullptr, false);
        xlnt_assert_equals(result, 2.3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_double_with_comma()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse("2,3", result, nullptr, true);
        xlnt_assert_equals(result, 2.3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_double_large()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        float result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse("1000000.5", result, nullptr, false);
        xlnt_assert_equals(result, 1000000.5);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_double_with_classic_locale()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        const char classic_decimal_separator = std::use_facet<std::numpunct<char>>(std::locale::classic()).decimal_point();
        std::string str = "2";
        str.push_back(classic_decimal_separator);
        str.push_back('3');
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result, nullptr, false);
        xlnt_assert_equals(result, 2.3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_double_with_system_locale()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        const char system_decimal_separator = std::use_facet<std::numpunct<char>>(xlnt::detail::get_system_locale()).decimal_point();
        std::string str = "2";
        str.push_back(system_decimal_separator);
        str.push_back('3');
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result, nullptr, true);
        xlnt_assert_equals(result, 2.3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_double_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "1.79769e+309";
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_double_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_double_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        double result = std::numeric_limits<double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_float_with_dot()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse("2.3", result, nullptr, false);
        xlnt_assert_equals(result, 2.3f);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_float_with_comma()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse("2,3", result, nullptr, true);
        xlnt_assert_equals(result, 2.3f);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_float_large()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse("1000000.5", result, nullptr, false);
        xlnt_assert_equals(result, 1000000.5f);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_float_with_classic_locale()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        const char classic_decimal_separator = std::use_facet<std::numpunct<char>>(std::locale::classic()).decimal_point();
        std::string str = "2";
        str.push_back(classic_decimal_separator);
        str.push_back('3');
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result, nullptr, false);
        xlnt_assert_equals(result, 2.3f);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_float_with_system_locale()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        const char system_decimal_separator = std::use_facet<std::numpunct<char>>(xlnt::detail::get_system_locale()).decimal_point();
        std::string str = "2";
        str.push_back(system_decimal_separator);
        str.push_back('3');
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result, nullptr, true);
        xlnt_assert_equals(result, 2.3f);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_float_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "3.40282e+39";
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<float>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_float_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<float>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_float_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        float result = std::numeric_limits<float>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<float>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_double_with_dot()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse("2.3", result, nullptr, false);
        xlnt_assert_equals(result, 2.3l);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_double_with_comma()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse("2,3", result, nullptr, true);
        xlnt_assert_equals(result, 2.3l);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_long_double_large()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        float result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse("1000000.5", result, nullptr, false);
        xlnt_assert_equals(result, 1000000.5l);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_double_with_classic_locale()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        const char classic_decimal_separator = std::use_facet<std::numpunct<char>>(std::locale::classic()).decimal_point();
        std::string str = "2";
        str.push_back(classic_decimal_separator);
        str.push_back('3');
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result, nullptr, false);
        xlnt_assert_equals(result, 2.3l);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_double_with_system_locale()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        const char system_decimal_separator = std::use_facet<std::numpunct<char>>(xlnt::detail::get_system_locale()).decimal_point();
        std::string str = "2";
        str.push_back(system_decimal_separator);
        str.push_back('3');
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result, nullptr, true);
        xlnt_assert_equals(result, 2.3l);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_long_double_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        // Note: the precision of "long double" depends on the system, so the following test
        // might need to be adapted in the future.
        std::string str = "1.18973e+4933";
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<long double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_long_double_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<long double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_double_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        long double result = std::numeric_limits<long double>::quiet_NaN();
        bool ok = xlnt::detail::parse(str, result);
        xlnt_assert_differs(result, std::numeric_limits<long double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_int()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        int i = -1;
        bool ok = xlnt::detail::parse("-5", i);
        xlnt_assert_equals(i, -5);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_int_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "2147483648";
        int i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_int_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        int i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_int_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        int i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_long()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        long i = -1;
        bool ok = xlnt::detail::parse("-5", i);
        xlnt_assert_equals(i, -5);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_long_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        // Note: this will ensure that "long" will be out of range both on systems
        // trating it as 32-bit (Windows) and as 64-bit (Linux / macOS).
        std::string str = "9223372036854775808";
        long i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_long_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        long i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        long i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_long_long()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        long long i = -1;
        bool ok = xlnt::detail::parse("-5", i);
        xlnt_assert_equals(i, -5);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_long_long_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "9223372036854775808";
        long long i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_long_long_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        long long i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_long_long_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        long long i = -1;
        bool ok = xlnt::detail::parse(str, i);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_unsigned_int()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned int n = 7;
        bool ok = xlnt::detail::parse("3", n);
        xlnt_assert_equals(n, 3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_unsigned_int_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "4294967296";
        unsigned int n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_unsigned_int_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        unsigned int n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_unsigned_int_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        unsigned int n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_unsigned_int_minus_number()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned int n = 7;
        bool ok = xlnt::detail::parse("-1", n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  ERANGE); // negative number -> out of range!
    }

    void test_parse_unsigned_int_minus_string()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned int n = 7;
        bool ok = xlnt::detail::parse("-blabla", n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_unsigned_long()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned long n = 7;
        bool ok = xlnt::detail::parse("3", n);
        xlnt_assert_equals(n, 3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_unsigned_long_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        // Note: this will ensure that "unsigned long" will be out of range both on systems
        // trating it as 32-bit (Windows) and as 64-bit (Linux / macOS).
        std::string str = "18446744073709551616";
        unsigned long n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_unsigned_long_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        unsigned long n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_unsigned_long_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        unsigned long n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_unsigned_long_minus_number()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned long n = 7;
        bool ok = xlnt::detail::parse("-1", n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  ERANGE); // negative number -> out of range!
    }

    void test_parse_unsigned_long_minus_string()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned long n = 7;
        bool ok = xlnt::detail::parse("-blabla", n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_unsigned_long_long()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned long long n = 7;
        bool ok = xlnt::detail::parse("3", n);
        xlnt_assert_equals(n, 3);
        xlnt_assert(ok);
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }
    
    void test_parse_unsigned_long_long_out_of_range()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "18446744073709551616";
        unsigned long long n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno == ERANGE); // no under/overflow occurred
    }

    void test_parse_unsigned_long_long_empty()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str;
        unsigned long long n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  0); // no under/overflow occurred
    }

    void test_parse_unsigned_long_long_text()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        std::string str = "  \n   \r  \t bla bla \n\r";
        unsigned long long n = 7;
        bool ok = xlnt::detail::parse(str, n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

    void test_parse_unsigned_long_long_minus_number()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned long long n = 7;
        bool ok = xlnt::detail::parse("-1", n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        xlnt_assert(errno ==  ERANGE); // negative number -> out of range!
    }

    void test_parse_unsigned_long_long_minus_string()
    {
        errno = -1; // for testing whether errno gets cleared correctly
        unsigned long long n = 7;
        bool ok = xlnt::detail::parse("-blabla", n);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert(!ok); // must fail
        //xlnt_assert(errno ==  0); // DISABLED, as errno is set on GCC
    }

};
static parser_test_suite x;
