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

#include <detail/parsers.hpp>
#include "helpers/assertions.hpp"

#include <helpers/test_suite.hpp>

class parser_test_suite : public test_suite
{
public:
    parser_test_suite()
    {
        register_test(test_parse_double_with_dot);
        register_test(test_parse_double_with_comma);
        register_test(test_parse_double_large);
        register_test(test_parse_double_out_of_range);
        register_test(test_parse_double_empty);
        register_test(test_parse_double_text);
        register_test(test_parse_float_with_dot);
        register_test(test_parse_float_with_comma);
        register_test(test_parse_float_large);
        register_test(test_parse_float_out_of_range);
        register_test(test_parse_float_empty);
        register_test(test_parse_float_text);
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
        double result = std::numeric_limits<double>::quiet_NaN();
        const char *str = "2.3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, result, &parsed_end);
        xlnt_assert_equals(result, 2.3);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_double_with_comma()
    {
        double result = std::numeric_limits<double>::quiet_NaN();
        const char *str = "2,3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, result, &parsed_end, ',');
        xlnt_assert_equals(result, 2.3);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_double_large()
    {
        float result = std::numeric_limits<double>::quiet_NaN();
        const char *str = "1000000.5";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, result, &parsed_end);
        xlnt_assert_equals(result, 1000000.5);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_double_out_of_range()
    {
        std::string str = "1.79769e+309";
        size_t parsed_length = 0;
        double result = std::numeric_limits<double>::quiet_NaN();
        std::errc error = xlnt::detail::parse(str, result, &parsed_length);
        xlnt_assert_differs(result, std::numeric_limits<double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_double_empty()
    {
        std::string str;
        size_t parsed_length = 0;
        double result = std::numeric_limits<double>::quiet_NaN();
        std::errc error = xlnt::detail::parse(str, result, &parsed_length);
        xlnt_assert_differs(result, std::numeric_limits<double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_double_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        size_t parsed_length = 0;
        double result = std::numeric_limits<double>::quiet_NaN();
        std::errc error = xlnt::detail::parse(str, result, &parsed_length);
        xlnt_assert_differs(result, std::numeric_limits<double>::quiet_NaN()); // NaN values are never equal
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_float_with_dot()
    {
        float result = std::numeric_limits<float>::quiet_NaN();
        const char *str = "2.3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, result, &parsed_end);
        xlnt_assert_equals(result, 2.3f);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_float_with_comma()
    {
        float result = std::numeric_limits<float>::quiet_NaN();
        const char *str = "2,3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, result, &parsed_end, ',');
        xlnt_assert_equals(result, 2.3f);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_float_large()
    {
        float result = std::numeric_limits<float>::quiet_NaN();
        const char *str = "1000000.5";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, result, &parsed_end);
        xlnt_assert_equals(result, 1000000.5f);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_float_out_of_range()
    {
        std::string str = "3.40282e+39";
        size_t parsed_length = 0;
        float result = std::numeric_limits<float>::quiet_NaN();
        std::errc error = xlnt::detail::parse(str, result, &parsed_length);
        xlnt_assert_differs(result, std::numeric_limits<float>::quiet_NaN()); // NaN values are never equal
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_float_empty()
    {
        std::string str;
        size_t parsed_length = 0;
        float result = std::numeric_limits<float>::quiet_NaN();
        std::errc error = xlnt::detail::parse(str, result, &parsed_length);
        xlnt_assert_differs(result, std::numeric_limits<float>::quiet_NaN()); // NaN values are never equal
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_float_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        size_t parsed_length = 0;
        float result = std::numeric_limits<float>::quiet_NaN();
        std::errc error = xlnt::detail::parse(str, result, &parsed_length);
        xlnt_assert_differs(result, std::numeric_limits<float>::quiet_NaN()); // NaN values are never equal
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_int()
    {
        int i = -1;
        const char *str = "-5";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, i, &parsed_end);
        xlnt_assert_equals(i, -5);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_int_out_of_range()
    {
        std::string str = "2147483648";
        int i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_int_empty()
    {
        std::string str;
        int i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_int_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        int i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_long()
    {
        long i = -1;
        const char *str = "-5";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, i, &parsed_end);
        xlnt_assert_equals(i, -5);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_long_out_of_range()
    {
        // Note: this will ensure that "long" will be out of range both on systems
        // trating it as 32-bit (Windows) and as 64-bit (Linux / macOS).
        std::string str = "9223372036854775808";
        long i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_long_empty()
    {
        std::string str;
        long i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_long_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        long i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_long_long()
    {
        long long i = -1;
        const char *str = "-5";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, i, &parsed_end);
        xlnt_assert_equals(i, -5);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_long_long_out_of_range()
    {
        std::string str = "9223372036854775808";
        long long i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_long_long_empty()
    {
        std::string str;
        long long i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_long_long_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        long long i = -1;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, i, &parsed_length);
        xlnt_assert_equals(i, -1); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_unsigned_int()
    {
        unsigned int n = 7;
        const char *str = "3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 3);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_unsigned_int_out_of_range()
    {
        std::string str = "4294967296";
        unsigned int n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_unsigned_int_empty()
    {
        std::string str;
        unsigned int n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_unsigned_int_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        unsigned int n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_unsigned_int_minus_number()
    {
        unsigned int n = 7;
        const char *str = "-1";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str, parsed_end);
    }

    void test_parse_unsigned_int_minus_string()
    {
        unsigned int n = 7;
        const char *str = "-blabla";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str, parsed_end);
    }

    void test_parse_unsigned_long()
    {
        unsigned long n = 7;
        const char *str = "3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 3);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_unsigned_long_out_of_range()
    {
        // Note: this will ensure that "unsigned long" will be out of range both on systems
        // trating it as 32-bit (Windows) and as 64-bit (Linux / macOS).
        std::string str = "18446744073709551616";
        unsigned long n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_unsigned_long_empty()
    {
        std::string str;
        unsigned long n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_unsigned_long_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        unsigned long n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_unsigned_long_minus_number()
    {
        unsigned long n = 7;
        const char *str = "-1";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str, parsed_end);
    }

    void test_parse_unsigned_long_minus_string()
    {
        unsigned long n = 7;
        const char *str = "-blabla";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str, parsed_end);
    }

    void test_parse_unsigned_long_long()
    {
        unsigned long long n = 7;
        const char *str = "3";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 3);
        xlnt_assert_equals(error, std::errc());
        xlnt_assert_equals(end, parsed_end);
    }

    void test_parse_unsigned_long_long_out_of_range()
    {
        std::string str = "18446744073709551616";
        unsigned long long n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::result_out_of_range); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_unsigned_long_long_empty()
    {
        std::string str;
        unsigned long long n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str.length(), parsed_length);
    }

    void test_parse_unsigned_long_long_text()
    {
        std::string str = "  \n   \r  \t bla bla \n\r";
        unsigned long long n = 7;
        size_t parsed_length = 0;
        std::errc error = xlnt::detail::parse(str, n, &parsed_length);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(parsed_length, 11);
    }

    void test_parse_unsigned_long_long_minus_number()
    {
        unsigned long long n = 7;
        const char *str = "-1";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str, parsed_end);
    }

    void test_parse_unsigned_long_long_minus_string()
    {
        unsigned long long n = 7;
        const char *str = "-blabla";
        const char *end = str + strlen(str);
        const char *parsed_end = nullptr;
        std::errc error = xlnt::detail::parse(str, n, &parsed_end);
        xlnt_assert_equals(n, 7); // expectation: leave unchanged
        xlnt_assert_equals(error, std::errc::invalid_argument); // must fail
        xlnt_assert_equals(str, parsed_end);
    }

};
static parser_test_suite x;
