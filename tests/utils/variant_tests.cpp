// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2024-2026 xlnt-community
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

#include <xlnt/utils/variant.hpp>
#include <xlnt/utils/datetime.hpp>
#include <helpers/test_suite.hpp>
#include <unordered_set>

class variant_test_suite : public test_suite
{
public:
    variant_test_suite()
        : test_suite()
    {
        register_test(test_null);
        register_test(test_empty);
        register_test(test_i1);
        register_test(test_i2);
        register_test(test_i4);
        register_test(test_integer);
        register_test(test_i8);
        register_test(test_ui1);
        register_test(test_ui2);
        register_test(test_ui4);
        register_test(test_uint);
        register_test(test_ui8);
        register_test(test_r4);
        register_test(test_r8);
        register_test(test_lpstr_c_string);
        register_test(test_lpstr_std_string);
        register_test(test_lpwstr_c_string);
        register_test(test_lpwstr_std_string);
        register_test(test_bstr_c_string);
        register_test(test_bstr_std_string);
        register_test(test_date);
        register_test(test_filetime);
        register_test(test_bool);
        register_test(test_i1_vector_from_initializer_list);
        register_test(test_i1_vector_from_vector);
        register_test(test_i2_vector_from_initializer_list);
        register_test(test_i2_vector_from_vector);
        register_test(test_i4_vector_from_initializer_list);
        register_test(test_i4_vector_from_vector);
        register_test(test_i8_vector_from_initializer_list);
        register_test(test_i8_vector_from_vector);
        register_test(test_ui1_vector_from_initializer_list);
        register_test(test_ui1_vector_from_vector);
        register_test(test_ui2_vector_from_initializer_list);
        register_test(test_ui2_vector_from_vector);
        register_test(test_ui4_vector_from_initializer_list);
        register_test(test_ui4_vector_from_vector);
        register_test(test_ui8_vector_from_initializer_list);
        register_test(test_ui8_vector_from_vector);
        register_test(test_r4_vector_from_initializer_list);
        register_test(test_r4_vector_from_vector);
        register_test(test_r8_vector_from_initializer_list);
        register_test(test_r8_vector_from_vector);
        register_test(test_lpstr_c_string_vector_from_initializer_list);
        register_test(test_lpstr_c_string_vector_from_vector);
        register_test(test_lpstr_std_string_vector_from_initializer_list);
        register_test(test_lpstr_std_string_vector_from_vector);
        register_test(test_lpwstr_c_string_vector_from_initializer_list);
        register_test(test_lpwstr_c_string_vector_from_vector);
        register_test(test_lpwstr_std_string_vector_from_initializer_list);
        register_test(test_lpwstr_std_string_vector_from_vector);
        register_test(test_bstr_c_string_vector_from_initializer_list);
        register_test(test_bstr_c_string_vector_from_vector);
        register_test(test_bstr_std_string_vector_from_initializer_list);
        register_test(test_bstr_std_string_vector_from_vector);
        register_test(test_date_vector_from_initializer_list);
        register_test(test_date_vector_from_vector);
        register_test(test_filetime_vector_from_initializer_list);
        register_test(test_filetime_vector_from_vector);
        register_test(test_bool_vector_from_initializer_list);
        register_test(test_bool_vector_from_vector);
        register_test(test_variant_vector_from_initializer_list);
        register_test(test_variant_vector_from_vector);
        register_test(test_variant_get_type_string);
        register_test(test_variant_get_base_type_string);
        register_test(test_variant_parse_type_string);
        register_test(test_variant_parse_base_type_string);
        register_test(test_operator_equal);
        register_test(test_construct_with_invalid_type);
        register_test(test_construct_vector_with_invalid_type);
    }

    template<typename T>
    void test_get_throw(const xlnt::variant &var_vec)
    {
        if (!std::is_same<T, std::int8_t>())
        {
            xlnt_assert_throws(var_vec.get<std::int8_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::int16_t>())
        {
            xlnt_assert_throws(var_vec.get<std::int16_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::int32_t>())
        {
            xlnt_assert_throws(var_vec.get<std::int32_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::int64_t>())
        {
            xlnt_assert_throws(var_vec.get<std::int64_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::uint8_t>())
        {
            xlnt_assert_throws(var_vec.get<std::uint8_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::uint16_t>())
        {
            xlnt_assert_throws(var_vec.get<std::uint16_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::uint32_t>())
        {
            xlnt_assert_throws(var_vec.get<std::uint32_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::uint64_t>())
        {
            xlnt_assert_throws(var_vec.get<std::uint64_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, float>())
        {
            xlnt_assert_throws(var_vec.get<float>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, double>())
        {
            xlnt_assert_throws(var_vec.get<double>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::string>() &&
            // DEPRECATED, will no longer be allowed in a future major version
            !std::is_same<T, xlnt::datetime>())
        {
            xlnt_assert_throws(var_vec.get<std::string>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, xlnt::datetime>())
        {
            xlnt_assert_throws(var_vec.get<xlnt::datetime>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, bool>())
        {
            xlnt_assert_throws(var_vec.get<bool>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<xlnt::variant>>() &&
            // The additional checks below are DEPRECATED and will no longer be allowed in a future major version.
            !std::is_same<T, std::vector<std::int32_t>>() &&
            !std::is_same<T, std::vector<std::string>>() &&
            !std::is_same<T, std::vector<xlnt::datetime>>() &&
            !std::is_same<T, std::vector<bool>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<xlnt::variant>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::int8_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::int8_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::int16_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::int16_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::int32_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::int32_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::int64_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::int64_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::uint8_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::uint8_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::uint16_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::uint16_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::uint32_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::uint32_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::uint64_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::uint64_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<float>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<float>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<double>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<double>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::string>>() &&
            // DEPRECATED, will no longer be allowed in a future major version
            !std::is_same<T, std::vector<xlnt::datetime>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::string>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<xlnt::datetime>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<xlnt::datetime>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<bool>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<bool>>(), xlnt::bad_variant_access);
        }
    }

    template <typename T, typename std::enable_if<
            std::is_same<T, std::int32_t>::value ||
            std::is_same<T, std::uint32_t>::value ||
            std::is_same<T, std::string>::value ||
            std::is_same<T, xlnt::datetime>::value
        >::type* = nullptr>
    void compare_variant(const xlnt::variant &var, const T &value, xlnt::variant::type expected_type)
    {
        xlnt::variant var_to_compare(value, expected_type);
        xlnt_assert(var == var_to_compare);
        xlnt_assert(!(var != var_to_compare));
    }

    template <typename T, typename std::enable_if<
            !std::is_same<T, std::int32_t>::value &&
            !std::is_same<T, std::uint32_t>::value &&
            !std::is_same<T, std::string>::value &&
            !std::is_same<T, xlnt::datetime>::value
        >::type* = nullptr>
    void compare_variant(const xlnt::variant &var, const T &value, xlnt::variant::type expected_type)
    {
        xlnt::variant var_to_compare(value);
        xlnt_assert(var == var_to_compare);
        xlnt_assert(!(var != var_to_compare));
    }

    template <typename T, typename std::enable_if<
            std::is_same<T, std::string>::value ||
            std::is_same<T, xlnt::datetime>::value
        >::type* = nullptr>
    void compare_variant_vector(const xlnt::variant &var, const std::vector<T> &vec, xlnt::variant::base_type expected_base_type)
    {
        xlnt::variant var_to_compare(vec, expected_base_type);
        xlnt_assert(var == var_to_compare);
        xlnt_assert(!(var != var_to_compare));
    }

    template <typename T, typename std::enable_if<
            !std::is_same<T, std::string>::value &&
            !std::is_same<T, xlnt::datetime>::value
        >::type* = nullptr>
    void compare_variant_vector(const xlnt::variant &var, const std::vector<T> &vec, xlnt::variant::base_type expected_base_type)
    {
        xlnt::variant var_to_compare(vec);
        xlnt_assert(var == var_to_compare);
        xlnt_assert(!(var != var_to_compare));
    }

    void test_no_elements(
        const xlnt::variant &var,
        xlnt::variant::type expected_type,
        xlnt::variant::base_type expected_base_type)
    {
        xlnt_assert_equals(var.value_type(), expected_type);
        xlnt_assert(var.is(expected_type));
        xlnt_assert_equals(var.value_base_type(), expected_base_type);
        xlnt_assert_equals(var.num_elements(), 0);
        test_get_throw<void>(var); // "null" and "empty" have no actual type
    }

    template<typename T>
    void test_single_element(
        const xlnt::variant &var,
        const T &expected_value,
        xlnt::variant::type expected_type,
        xlnt::variant::base_type expected_base_type)
    {
        xlnt_assert_equals(var.value_type(), expected_type);
        xlnt_assert(var.is(expected_type));
        xlnt_assert_equals(var.value_base_type(), expected_base_type);
        xlnt_assert_equals(var.num_elements(), 1);
        xlnt_assert_equals(expected_value, var.get<T>());
        test_get_throw<T>(var);
        compare_variant<T>(var, expected_value, expected_type);
    }

    void test_null()
    {
        // IMPORTANT: the default type is "null". This test implicitly also
        // tests whether the default changes.
        xlnt::variant var_null;
        test_no_elements(var_null, xlnt::variant::type::null, xlnt::variant::base_type::variant);
    }

    void test_empty()
    {
        xlnt::variant var_empty(xlnt::variant::type::empty);
        test_no_elements(var_empty, xlnt::variant::type::empty, xlnt::variant::base_type::variant);
    }

    void test_i1()
    {
        constexpr std::int8_t value = 10;
        xlnt::variant var_i1(value);
        test_single_element(var_i1, value, xlnt::variant::type::i1, xlnt::variant::base_type::i1);
    }

    void test_i2()
    {
        constexpr std::int16_t value = 10;
        xlnt::variant var_i2(value);
        test_single_element(var_i2, value, xlnt::variant::type::i2, xlnt::variant::base_type::i2);
    }

    void test_i4()
    {
        constexpr std::int32_t value = 10;
        // IMPORTANT: the default type is "i4". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_i4(value);
        test_single_element(var_i4, value, xlnt::variant::type::i4, xlnt::variant::base_type::i4);
    }

    void test_integer()
    {
        constexpr std::int32_t value = 10;
        xlnt::variant var_integer(value, xlnt::variant::type::integer);
        test_single_element(var_integer, value, xlnt::variant::type::integer, xlnt::variant::base_type::i4);
    }

    void test_i8()
    {
        constexpr std::int64_t value = 10;
        xlnt::variant var_i8(value);
        test_single_element(var_i8, value, xlnt::variant::type::i8, xlnt::variant::base_type::i8);
    }

    void test_ui1()
    {
        constexpr std::uint8_t value = 10;
        xlnt::variant var_ui1(value);
        test_single_element(var_ui1, value, xlnt::variant::type::ui1, xlnt::variant::base_type::ui1);
    }

    void test_ui2()
    {
        constexpr std::uint16_t value = 10;
        xlnt::variant var_ui2(value);
        test_single_element(var_ui2, value, xlnt::variant::type::ui2, xlnt::variant::base_type::ui2);
    }

    void test_ui4()
    {
        constexpr std::uint32_t value = 10;
        // IMPORTANT: the default type is "ui4". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_ui4(value);
        test_single_element(var_ui4, value, xlnt::variant::type::ui4, xlnt::variant::base_type::ui4);
    }

    void test_uint()
    {
        constexpr std::uint32_t value = 10;
        xlnt::variant var_uint(value, xlnt::variant::type::uint);
        test_single_element(var_uint, value, xlnt::variant::type::uint, xlnt::variant::base_type::ui4);
    }

    void test_ui8()
    {
        constexpr std::uint64_t value = 10;
        xlnt::variant var_ui8(value);
        test_single_element(var_ui8, value, xlnt::variant::type::ui8, xlnt::variant::base_type::ui8);
    }

    void test_r4()
    {
        constexpr float value = 0.3f;
        xlnt::variant var_r4(value);
        test_single_element(var_r4, value, xlnt::variant::type::r4, xlnt::variant::base_type::r4);
    }

    void test_r8()
    {
        constexpr double value = 0.3;
        xlnt::variant var_r8(value);
        test_single_element(var_r8, value, xlnt::variant::type::r8, xlnt::variant::base_type::r8);
    }

    void test_lpstr_c_string()
    {
        // IMPORTANT: the default type is "lpstr". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_c_str("test1");
        test_single_element(var_c_str, std::string{"test1"}, xlnt::variant::type::lpstr, xlnt::variant::base_type::lpstr);
    }

    void test_lpstr_std_string()
    {
        // IMPORTANT: the default type is "lpstr". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_std_str(std::string("test2"));
        test_single_element(var_std_str, std::string{"test2"}, xlnt::variant::type::lpstr, xlnt::variant::base_type::lpstr);
    }

    void test_lpwstr_c_string()
    {
        xlnt::variant var_c_str("test1", xlnt::variant::type::lpwstr);
        test_single_element(var_c_str, std::string{"test1"}, xlnt::variant::type::lpwstr, xlnt::variant::base_type::lpwstr);
    }

    void test_lpwstr_std_string()
    {
        xlnt::variant var_std_str(std::string("test2"), xlnt::variant::type::lpwstr);
        test_single_element(var_std_str, std::string{"test2"}, xlnt::variant::type::lpwstr, xlnt::variant::base_type::lpwstr);
    }

    void test_bstr_c_string()
    {
        xlnt::variant var_c_str("test1", xlnt::variant::type::bstr);
        test_single_element(var_c_str, std::string{"test1"}, xlnt::variant::type::bstr, xlnt::variant::base_type::bstr);
    }

    void test_bstr_std_string()
    {
        xlnt::variant var_std_str(std::string("test2"), xlnt::variant::type::bstr);
        test_single_element(var_std_str, std::string{"test2"}, xlnt::variant::type::bstr, xlnt::variant::base_type::bstr);
    }

    void test_date()
    {
        const xlnt::datetime datetime(2025, 8, 15, 17, 30, 45, 123456);
        // IMPORTANT: the default type is "date". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_date(datetime);
        test_single_element(var_date, datetime, xlnt::variant::type::date, xlnt::variant::base_type::date);
    }

    void test_filetime()
    {
        const xlnt::datetime datetime(2025, 8, 15, 17, 30, 45, 123456);
        xlnt::variant var_filetime(datetime, xlnt::variant::type::filetime);
        test_single_element(var_filetime, datetime, xlnt::variant::type::filetime, xlnt::variant::base_type::filetime);
    }

    void test_bool()
    {
        xlnt::variant var_bool(true);
        test_single_element(var_bool, true, xlnt::variant::type::boolean, xlnt::variant::base_type::boolean);
    }

    template<typename T>
    void test_variant_vector(
        const xlnt::variant &var_vec,
        const std::vector<T>& vec,
        xlnt::variant::base_type expected_base_type)
    {
        xlnt_assert_equals(var_vec.value_type(), xlnt::variant::type::vector);
        xlnt_assert(var_vec.is(xlnt::variant::type::vector));
        xlnt_assert_equals(var_vec.value_base_type(), expected_base_type);
        xlnt_assert_equals(var_vec.num_elements(), vec.size());
        xlnt_assert_equals(var_vec.get<std::vector<T>>(), vec);
        test_get_throw<std::vector<T>>(var_vec);
        compare_variant_vector<T>(var_vec, vec, expected_base_type);
    }

    template<typename T>
    void test_vector_from_initializer_list(
        const std::initializer_list<T>& list,
        xlnt::variant::base_type expected_base_type)
    {
        xlnt::variant var_vec(list);
        test_variant_vector<T>(var_vec, std::vector<T>(list), expected_base_type);
    }

    template<typename T>
    void test_vector_from_vector(
        const std::vector<T>& vec,
        xlnt::variant::base_type expected_base_type)
    {
        xlnt::variant var_vec(vec);
        test_variant_vector<T>(var_vec, vec, expected_base_type);
    }

    void test_c_string_vector_from_initializer_list(
        const xlnt::variant &var_vec,
        const std::initializer_list<const char *>& list,
        xlnt::variant::base_type expected_base_type)
    {
        std::vector<std::string> vec_str;
        vec_str.reserve(list.size());
        for (const auto &elem : list)
        {
            vec_str.emplace_back(elem);
        }
        test_variant_vector(var_vec, vec_str, expected_base_type);
    }

    void test_c_string_vector_from_vector(
        const xlnt::variant &var_vec,
        const std::vector<const char *>& vec,
        xlnt::variant::base_type expected_base_type)
    {
        std::vector<std::string> vec_str;
        vec_str.reserve(vec.size());
        for (const auto &elem : vec)
        {
            vec_str.emplace_back(elem);
        }
        test_variant_vector(var_vec, vec_str, expected_base_type);
    }

    void test_i1_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::int8_t>{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()},
            xlnt::variant::base_type::i1);
    }

    void test_i1_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::int8_t>{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()},
            xlnt::variant::base_type::i1);
    }

    void test_i2_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::int16_t>{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()},
            xlnt::variant::base_type::i2);
    }

    void test_i2_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::int16_t>{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()},
            xlnt::variant::base_type::i2);
    }

    void test_i4_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()},
            xlnt::variant::base_type::i4);
    }

    void test_i4_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()},
            xlnt::variant::base_type::i4);
    }

    void test_i8_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::int64_t>{10, 20, 30, 40, std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()},
            xlnt::variant::base_type::i8);
    }

    void test_i8_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::int64_t>{10, 20, 30, 40, std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()},
            xlnt::variant::base_type::i8);
    }

    void test_ui1_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::uint8_t>{10, 20, 30, 40, std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max()},
            xlnt::variant::base_type::ui1);
    }

    void test_ui1_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::uint8_t>{10, 20, 30, 40, std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max()},
            xlnt::variant::base_type::ui1);
    }

    void test_ui2_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::uint16_t>{10, 20, 30, 40, std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max()},
            xlnt::variant::base_type::ui2);
    }

    void test_ui2_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::uint16_t>{10, 20, 30, 40, std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max()},
            xlnt::variant::base_type::ui2);
    }

    void test_ui4_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::uint32_t>{10, 20, 30, 40, std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max()},
            xlnt::variant::base_type::ui4);
    }

    void test_ui4_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::uint32_t>{10, 20, 30, 40, std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max()},
            xlnt::variant::base_type::ui4);
    }

    void test_ui8_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<std::uint64_t>{10, 20, 30, 40, std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max()},
            xlnt::variant::base_type::ui8);
    }

    void test_ui8_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<std::uint64_t>{10, 20, 30, 40, std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max()},
            xlnt::variant::base_type::ui8);
    }

    void test_r4_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<float>{0.1f, 0.2f, 0.3f, 0.4f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()},
            xlnt::variant::base_type::r4);
    }

    void test_r4_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<float>{0.1f, 0.2f, 0.3f, 0.4f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()},
            xlnt::variant::base_type::r4);
    }

    void test_r8_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(
            std::initializer_list<double>{0.1, 0.2, 0.3, 0.4, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max()},
            xlnt::variant::base_type::r8);
    }

    void test_r8_vector_from_vector()
    {
        test_vector_from_vector(
            std::vector<double>{0.1, 0.2, 0.3, 0.4, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max()},
            xlnt::variant::base_type::r8);
    }

    void test_lpstr_c_string_vector_from_initializer_list()
    {
        std::initializer_list<const char *> list{"test1", "test2", "test3", "test4"};
        // IMPORTANT: the default type is "lpstr". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_vec(list);
        test_c_string_vector_from_initializer_list(var_vec, list, xlnt::variant::base_type::lpstr);
    }

    void test_lpstr_c_string_vector_from_vector()
    {
        std::vector<const char *> vec{"test1", "test2", "test3", "test4"};
        // IMPORTANT: the default type is "lpstr". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_vec(vec);
        test_c_string_vector_from_vector(var_vec, vec, xlnt::variant::base_type::lpstr);
    }

    void test_lpstr_std_string_vector_from_initializer_list()
    {
        std::initializer_list<std::string> list{"test1", "test2", "test3", "test4"};
        // IMPORTANT: the default type is "lpstr". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_vec(list);
        test_variant_vector<std::string>(var_vec, std::vector<std::string>(list), xlnt::variant::base_type::lpstr);
    }

    void test_lpstr_std_string_vector_from_vector()
    {
        std::vector<std::string> vec{"test1", "test2", "test3", "test4"};
        // IMPORTANT: the default type is "lpstr". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_vec(vec);
        test_variant_vector<std::string>(var_vec, vec, xlnt::variant::base_type::lpstr);
    }

    void test_lpwstr_c_string_vector_from_initializer_list()
    {
        std::initializer_list<const char *> list{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(list, xlnt::variant::base_type::lpwstr);
        test_c_string_vector_from_initializer_list(var_vec, list, xlnt::variant::base_type::lpwstr);
    }

    void test_lpwstr_c_string_vector_from_vector()
    {
        std::vector<const char *> vec{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(vec, xlnt::variant::base_type::lpwstr);
        test_c_string_vector_from_vector(var_vec, vec, xlnt::variant::base_type::lpwstr);
    }

    void test_lpwstr_std_string_vector_from_initializer_list()
    {
        std::initializer_list<std::string> list{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(list, xlnt::variant::base_type::lpwstr);
        test_variant_vector<std::string>(var_vec, std::vector<std::string>(list), xlnt::variant::base_type::lpwstr);
    }

    void test_lpwstr_std_string_vector_from_vector()
    {
        std::vector<std::string> vec{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(vec, xlnt::variant::base_type::lpwstr);
        test_variant_vector<std::string>(var_vec, vec, xlnt::variant::base_type::lpwstr);
    }

    void test_bstr_c_string_vector_from_initializer_list()
    {
        std::initializer_list<const char *> list{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(list, xlnt::variant::base_type::bstr);
        test_c_string_vector_from_initializer_list(var_vec, list, xlnt::variant::base_type::bstr);
    }

    void test_bstr_c_string_vector_from_vector()
    {
        std::vector<const char *> vec{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(vec, xlnt::variant::base_type::bstr);
        test_c_string_vector_from_vector(var_vec, vec, xlnt::variant::base_type::bstr);
    }

    void test_bstr_std_string_vector_from_initializer_list()
    {
        std::initializer_list<std::string> list{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(list, xlnt::variant::base_type::bstr);
        test_variant_vector<std::string>(var_vec, std::vector<std::string>(list), xlnt::variant::base_type::bstr);
    }

    void test_bstr_std_string_vector_from_vector()
    {
        std::vector<std::string> vec{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(vec, xlnt::variant::base_type::bstr);
        test_variant_vector<std::string>(var_vec, vec, xlnt::variant::base_type::bstr);
    }

    void test_date_vector_from_initializer_list()
    {
        std::initializer_list<xlnt::datetime> list{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)};
        // IMPORTANT: the default type is "date". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_date(list);
        test_variant_vector<xlnt::datetime>(var_date, std::vector<xlnt::datetime>(list), xlnt::variant::base_type::date);
    }

    void test_date_vector_from_vector()
    {
        std::vector<xlnt::datetime> vec{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)};
        // IMPORTANT: the default type is "date". This test implicitly also
        // tests whether the default ever changes.
        xlnt::variant var_date(vec);
        test_variant_vector<xlnt::datetime>(var_date, vec, xlnt::variant::base_type::date);
    }

    void test_filetime_vector_from_initializer_list()
    {
        std::initializer_list<xlnt::datetime> list{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)};
        xlnt::variant var_filetime(list, xlnt::variant::base_type::filetime);
        test_variant_vector<xlnt::datetime>(var_filetime, std::vector<xlnt::datetime>(list), xlnt::variant::base_type::filetime);
    }

    void test_filetime_vector_from_vector()
    {
        std::vector<xlnt::datetime> vec{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)};
        xlnt::variant var_filetime(vec, xlnt::variant::base_type::filetime);
        test_variant_vector<xlnt::datetime>(var_filetime, vec, xlnt::variant::base_type::filetime);
    }

    void test_bool_vector_from_initializer_list()
    {
        test_vector_from_initializer_list<bool>(
            std::initializer_list<bool>{true, false, false, true, true},
            xlnt::variant::base_type::boolean);
    }

    void test_bool_vector_from_vector()
    {
        test_vector_from_vector<bool>(
            std::vector<bool>{true, false, false, true, true},
            xlnt::variant::base_type::boolean);
    }

    void test_variant_vector_from_initializer_list()
    {
        // Test combinations of all types
        test_vector_from_initializer_list<xlnt::variant>(std::initializer_list<xlnt::variant>{
            // Test single elements for all types
            xlnt::variant(xlnt::variant::type::null),
            xlnt::variant(xlnt::variant::type::empty),
            xlnt::variant(std::int8_t(10)),
            xlnt::variant(std::int16_t(10)),
            xlnt::variant(std::int32_t(10), xlnt::variant::type::i4),
            xlnt::variant(std::int32_t(10), xlnt::variant::type::integer),
            xlnt::variant(std::int64_t(10)),
            xlnt::variant(std::uint8_t(10)),
            xlnt::variant(std::uint16_t(10)),
            xlnt::variant(std::uint32_t(10), xlnt::variant::type::ui4),
            xlnt::variant(std::uint32_t(10), xlnt::variant::type::uint),
            xlnt::variant(std::uint64_t(10)),
            xlnt::variant(0.3f),
            xlnt::variant(0.3),
            xlnt::variant("test1", xlnt::variant::type::lpstr),
            xlnt::variant(std::string("test1"), xlnt::variant::type::lpstr),
            xlnt::variant("test1", xlnt::variant::type::lpwstr),
            xlnt::variant(std::string("test1"), xlnt::variant::type::lpwstr),
            xlnt::variant("test1", xlnt::variant::type::bstr),
            xlnt::variant(std::string("test1"), xlnt::variant::type::bstr),
            xlnt::variant(xlnt::datetime(1975, 2, 14), xlnt::variant::type::date),
            xlnt::variant(xlnt::datetime(1975, 2, 14), xlnt::variant::type::filetime),
            xlnt::variant(true),

            // Test initializer lists for all types
            xlnt::variant(std::initializer_list<std::int8_t>{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()}),
            xlnt::variant(std::initializer_list<std::int16_t>{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()}),
            xlnt::variant(std::initializer_list<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::initializer_list<std::int64_t>{10, 20, 30, 40, std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint8_t>{10, 20, 30, 40, std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint16_t>{10, 20, 30, 40, std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint32_t>{10, 20, 30, 40, std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint64_t>{10, 20, 30, 40, std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max()}),
            xlnt::variant(std::initializer_list<float>{0.1f, 0.2f, 0.3f, 0.4f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()}),
            xlnt::variant(std::initializer_list<double>{0.1, 0.2, 0.3, 0.4, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max()}),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::date),
            xlnt::variant(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::filetime),
            xlnt::variant(std::initializer_list<bool>{true, false, false, true, true}),

            // Test vectors for all types
            xlnt::variant(std::vector<std::int8_t>{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()}),
            xlnt::variant(std::vector<std::int16_t>{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()}),
            xlnt::variant(std::vector<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::vector<std::int64_t>{10, 20, 30, 40, std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()}),
            xlnt::variant(std::vector<std::uint8_t>{10, 20, 30, 40, std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max()}),
            xlnt::variant(std::vector<std::uint16_t>{10, 20, 30, 40, std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max()}),
            xlnt::variant(std::vector<std::uint32_t>{10, 20, 30, 40, std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max()}),
            xlnt::variant(std::vector<std::uint64_t>{10, 20, 30, 40, std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max()}),
            xlnt::variant(std::vector<float>{0.1f, 0.2f, 0.3f, 0.4f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()}),
            xlnt::variant(std::vector<double>{0.1, 0.2, 0.3, 0.4, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max()}),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::date),
            xlnt::variant(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::filetime),
            xlnt::variant(std::vector<bool>{true, false, false, true, true}),
        },
        xlnt::variant::base_type::variant);
    }

    void test_variant_vector_from_vector()
    {
        // Test combinations of all types
        test_vector_from_vector<xlnt::variant>(std::vector<xlnt::variant>{
            // Test single elements for all types
            xlnt::variant(xlnt::variant::type::null),
            xlnt::variant(xlnt::variant::type::empty),
            xlnt::variant(std::int8_t(10)),
            xlnt::variant(std::int16_t(10)),
            xlnt::variant(std::int32_t(10), xlnt::variant::type::i4),
            xlnt::variant(std::int32_t(10), xlnt::variant::type::integer),
            xlnt::variant(std::int64_t(10)),
            xlnt::variant(std::uint8_t(10)),
            xlnt::variant(std::uint16_t(10)),
            xlnt::variant(std::uint32_t(10), xlnt::variant::type::ui4),
            xlnt::variant(std::uint32_t(10), xlnt::variant::type::uint),
            xlnt::variant(std::uint64_t(10)),
            xlnt::variant(0.3f),
            xlnt::variant(0.3),
            xlnt::variant("test1", xlnt::variant::type::lpstr),
            xlnt::variant(std::string("test1"), xlnt::variant::type::lpstr),
            xlnt::variant("test1", xlnt::variant::type::lpwstr),
            xlnt::variant(std::string("test1"), xlnt::variant::type::lpwstr),
            xlnt::variant("test1", xlnt::variant::type::bstr),
            xlnt::variant(std::string("test1"), xlnt::variant::type::bstr),
            xlnt::variant(xlnt::datetime(1975, 2, 14), xlnt::variant::type::date),
            xlnt::variant(xlnt::datetime(1975, 2, 14), xlnt::variant::type::filetime),
            xlnt::variant(true),

            // Test initializer lists for all types
            xlnt::variant(std::initializer_list<std::int8_t>{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()}),
            xlnt::variant(std::initializer_list<std::int16_t>{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()}),
            xlnt::variant(std::initializer_list<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::initializer_list<std::int64_t>{10, 20, 30, 40, std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint8_t>{10, 20, 30, 40, std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint16_t>{10, 20, 30, 40, std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint32_t>{10, 20, 30, 40, std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max()}),
            xlnt::variant(std::initializer_list<std::uint64_t>{10, 20, 30, 40, std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max()}),
            xlnt::variant(std::initializer_list<float>{0.1f, 0.2f, 0.3f, 0.4f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()}),
            xlnt::variant(std::initializer_list<double>{0.1, 0.2, 0.3, 0.4, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max()}),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::date),
            xlnt::variant(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::filetime),
            xlnt::variant(std::initializer_list<bool>{true, false, false, true, true}),

            // Test vectors for all types
            xlnt::variant(std::vector<std::int8_t>{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()}),
            xlnt::variant(std::vector<std::int16_t>{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()}),
            xlnt::variant(std::vector<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::vector<std::int64_t>{10, 20, 30, 40, std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()}),
            xlnt::variant(std::vector<std::uint8_t>{10, 20, 30, 40, std::numeric_limits<std::uint8_t>::min(), std::numeric_limits<std::uint8_t>::max()}),
            xlnt::variant(std::vector<std::uint16_t>{10, 20, 30, 40, std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max()}),
            xlnt::variant(std::vector<std::uint32_t>{10, 20, 30, 40, std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max()}),
            xlnt::variant(std::vector<std::uint64_t>{10, 20, 30, 40, std::numeric_limits<std::uint64_t>::min(), std::numeric_limits<std::uint64_t>::max()}),
            xlnt::variant(std::vector<float>{0.1f, 0.2f, 0.3f, 0.4f, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()}),
            xlnt::variant(std::vector<double>{0.1, 0.2, 0.3, 0.4, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::min(), std::numeric_limits<double>::max()}),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpstr),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::lpwstr),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}, xlnt::variant::base_type::bstr),
            xlnt::variant(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::date),
            xlnt::variant(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}, xlnt::variant::base_type::filetime),
            xlnt::variant(std::vector<bool>{true, false, false, true, true}),
        },
        xlnt::variant::base_type::variant);
    }

    void test_variant_get_type_string()
    {
        for (int type = 0; type <= 32; ++type)
        {
            xlnt::variant::type typed_type = static_cast<xlnt::variant::type>(type);
            switch (typed_type)
            {
            case xlnt::variant::type::vector:
            //case xlnt::variant::type::array:
            //case xlnt::variant::type::blob:
            //case xlnt::variant::type::oblob:
            case xlnt::variant::type::empty:
            case xlnt::variant::type::null:
            case xlnt::variant::type::i1:
            case xlnt::variant::type::i2:
            case xlnt::variant::type::i4:
            case xlnt::variant::type::i8:
            case xlnt::variant::type::integer:
            case xlnt::variant::type::ui1:
            case xlnt::variant::type::ui2:
            case xlnt::variant::type::ui4:
            case xlnt::variant::type::ui8:
            case xlnt::variant::type::uint:
            case xlnt::variant::type::r4:
            case xlnt::variant::type::r8:
            //case xlnt::variant::type::decimal:
            case xlnt::variant::type::lpstr:
            case xlnt::variant::type::lpwstr:
            case xlnt::variant::type::bstr:
            case xlnt::variant::type::date:
            case xlnt::variant::type::filetime:
            case xlnt::variant::type::boolean:
            //case xlnt::variant::type::cy:
            //case xlnt::variant::type::error:
            //case xlnt::variant::type::stream:
            //case xlnt::variant::type::ostream:
            //case xlnt::variant::type::storage:
            //case xlnt::variant::type::ostorage:
            //case xlnt::variant::type::vstream:
            //case xlnt::variant::type::clsid:
                xlnt_assert_throws_nothing(xlnt::variant::get_type_string(typed_type));
                break;
            default:
                xlnt_assert_throws(xlnt::variant::get_type_string(typed_type), xlnt::invalid_parameter);
                break;
            }
        }
        xlnt_assert_throws(xlnt::variant::get_type_string(static_cast<xlnt::variant::type>(-123456789)), xlnt::invalid_parameter);
    }

    void test_variant_get_base_type_string()
    {
        for (int type = 0; type <= 22; ++type)
        {
            xlnt::variant::base_type typed_base_type = static_cast<xlnt::variant::base_type>(type);
            switch (typed_base_type)
            {
            case xlnt::variant::base_type::variant:
            case xlnt::variant::base_type::i1:
            case xlnt::variant::base_type::i2:
            case xlnt::variant::base_type::i4:
            case xlnt::variant::base_type::i8:        // only for vectors
            //case xlnt::variant::base_type::integer: // only for arrays
            case xlnt::variant::base_type::ui1:
            case xlnt::variant::base_type::ui2:
            case xlnt::variant::base_type::ui4:
            case xlnt::variant::base_type::ui8:       // only for vectors
            //case xlnt::variant::base_type::uint:    // only for arrays
            case xlnt::variant::base_type::r4:
            case xlnt::variant::base_type::r8:
            //case xlnt::variant::base_type::decimal: // only for arrays
            case xlnt::variant::base_type::lpstr:     // only for vectors
            case xlnt::variant::base_type::lpwstr:    // only for vectors
            case xlnt::variant::base_type::bstr:
            case xlnt::variant::base_type::date:
            case xlnt::variant::base_type::filetime:  // only for vectors
            case xlnt::variant::base_type::boolean:
            //case xlnt::variant::base_type::cy:
            //case xlnt::variant::base_type::error:
            //case xlnt::variant::base_type::clsid:    // only for vectors
                xlnt_assert_throws_nothing(xlnt::variant::get_base_type_string(typed_base_type));
                break;
            default:
                xlnt_assert_throws(xlnt::variant::get_base_type_string(typed_base_type), xlnt::invalid_parameter);
                break;
            }
        }
        xlnt_assert_throws(xlnt::variant::get_base_type_string(static_cast<xlnt::variant::base_type>(-123456789)), xlnt::invalid_parameter);
    }

    void test_variant_parse_type_string()
    {
        xlnt_assert_equals(xlnt::variant::parse_type_string("vector"), xlnt::variant::type::vector);
        xlnt_assert_throws(xlnt::variant::parse_type_string("array"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("blob"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("oblob"), xlnt::invalid_parameter);
        xlnt_assert_equals(xlnt::variant::parse_type_string("empty"), xlnt::variant::type::empty);
        xlnt_assert_equals(xlnt::variant::parse_type_string("null"), xlnt::variant::type::null);
        xlnt_assert_equals(xlnt::variant::parse_type_string("i1"), xlnt::variant::type::i1);
        xlnt_assert_equals(xlnt::variant::parse_type_string("i2"), xlnt::variant::type::i2);
        xlnt_assert_equals(xlnt::variant::parse_type_string("i4"), xlnt::variant::type::i4);
        xlnt_assert_equals(xlnt::variant::parse_type_string("i8"), xlnt::variant::type::i8);
        xlnt_assert_equals(xlnt::variant::parse_type_string("int"), xlnt::variant::type::integer);
        xlnt_assert_equals(xlnt::variant::parse_type_string("ui1"), xlnt::variant::type::ui1);
        xlnt_assert_equals(xlnt::variant::parse_type_string("ui2"), xlnt::variant::type::ui2);
        xlnt_assert_equals(xlnt::variant::parse_type_string("ui4"), xlnt::variant::type::ui4);
        xlnt_assert_equals(xlnt::variant::parse_type_string("ui8"), xlnt::variant::type::ui8);
        xlnt_assert_equals(xlnt::variant::parse_type_string("uint"), xlnt::variant::type::uint);
        xlnt_assert_equals(xlnt::variant::parse_type_string("r4"), xlnt::variant::type::r4);
        xlnt_assert_equals(xlnt::variant::parse_type_string("r8"), xlnt::variant::type::r8);
        xlnt_assert_throws(xlnt::variant::parse_type_string("decimal"), xlnt::invalid_parameter);
        xlnt_assert_equals(xlnt::variant::parse_type_string("lpstr"), xlnt::variant::type::lpstr);
        xlnt_assert_equals(xlnt::variant::parse_type_string("lpwstr"), xlnt::variant::type::lpwstr);
        xlnt_assert_equals(xlnt::variant::parse_type_string("bstr"), xlnt::variant::type::bstr);
        xlnt_assert_equals(xlnt::variant::parse_type_string("date"), xlnt::variant::type::date);
        xlnt_assert_equals(xlnt::variant::parse_type_string("filetime"), xlnt::variant::type::filetime);
        xlnt_assert_equals(xlnt::variant::parse_type_string("bool"), xlnt::variant::type::boolean);
        xlnt_assert_throws(xlnt::variant::parse_type_string("cy"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("error"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("stream"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("ostream"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("storage"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("ostorage"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("vstream"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("clsid"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_type_string("INVALID"), xlnt::invalid_parameter);
    }

    void test_variant_parse_base_type_string()
    {
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("variant"), xlnt::variant::base_type::variant);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("i1"), xlnt::variant::base_type::i1);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("i2"), xlnt::variant::base_type::i2);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("i4"), xlnt::variant::base_type::i4);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("i8"), xlnt::variant::base_type::i8);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("int"), xlnt::invalid_parameter);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("ui1"), xlnt::variant::base_type::ui1);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("ui2"), xlnt::variant::base_type::ui2);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("ui4"), xlnt::variant::base_type::ui4);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("ui8"), xlnt::variant::base_type::ui8);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("uint"), xlnt::invalid_parameter);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("r4"), xlnt::variant::base_type::r4);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("r8"), xlnt::variant::base_type::r8);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("decimal"), xlnt::invalid_parameter);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("lpstr"), xlnt::variant::base_type::lpstr);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("lpwstr"), xlnt::variant::base_type::lpwstr);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("bstr"), xlnt::variant::base_type::bstr);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("date"), xlnt::variant::base_type::date);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("filetime"), xlnt::variant::base_type::filetime);
        xlnt_assert_equals(xlnt::variant::parse_base_type_string("bool"), xlnt::variant::base_type::boolean);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("cy"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("error"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("clsid"), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::variant::parse_base_type_string("INVALID"), xlnt::invalid_parameter);
    }

    void test_operator_equal()
    {
        constexpr std::int8_t value_i1 = 10;
        xlnt::variant var_i1(value_i1);
        xlnt_assert_equals(var_i1, xlnt::variant(value_i1));
        xlnt_assert_differs(var_i1, xlnt::variant(value_i1 + 1));

        constexpr std::int16_t value_i2 = 10;
        xlnt::variant var_i2(value_i2);
        xlnt_assert_equals(var_i2, xlnt::variant(value_i2));
        xlnt_assert_differs(var_i2, xlnt::variant(value_i2 + 1));

        xlnt_assert_differs(var_i1, var_i2);


        std::vector<std::int8_t> vec_i1{10, 20, 30, 40, std::numeric_limits<std::int8_t>::min(), std::numeric_limits<std::int8_t>::max()};
        xlnt::variant var_vec_i1(vec_i1);
        xlnt_assert_equals(var_vec_i1, xlnt::variant(vec_i1));
        auto vec_i1_modified = vec_i1;
        vec_i1_modified.at(0) += 1;
        xlnt_assert_differs(var_vec_i1, xlnt::variant(vec_i1_modified));

        std::vector<std::int16_t> vec_i2{10, 20, 30, 40, std::numeric_limits<std::int16_t>::min(), std::numeric_limits<std::int16_t>::max()};
        xlnt::variant var_vec_i2(vec_i2);
        xlnt_assert_equals(var_vec_i2, xlnt::variant(vec_i2));

        auto vec_i2_modified = vec_i2;
        vec_i2_modified.at(0) += 1;
        xlnt_assert_differs(var_vec_i2, xlnt::variant(vec_i2_modified));

        xlnt_assert_differs(var_vec_i1, var_vec_i2);
    }

    const std::vector<xlnt::variant::type> get_all_types()
    {
        return {
            xlnt::variant::type::vector,
            //xlnt::variant::type::array,
            //xlnt::variant::type::blob,
            //xlnt::variant::type::oblob,
            xlnt::variant::type::empty,
            xlnt::variant::type::null,
            xlnt::variant::type::i1,
            xlnt::variant::type::i2,
            xlnt::variant::type::i4,
            xlnt::variant::type::i8,
            xlnt::variant::type::integer,
            xlnt::variant::type::ui1,
            xlnt::variant::type::ui2,
            xlnt::variant::type::ui4,
            xlnt::variant::type::ui8,
            xlnt::variant::type::uint,
            xlnt::variant::type::r4,
            xlnt::variant::type::r8,
            //xlnt::variant::type::decimal,
            xlnt::variant::type::lpstr,
            xlnt::variant::type::lpwstr,
            xlnt::variant::type::bstr,
            xlnt::variant::type::date,
            xlnt::variant::type::filetime,
            xlnt::variant::type::boolean,
            //xlnt::variant::type::cy,
            //xlnt::variant::type::error,
            //xlnt::variant::type::stream,
            //xlnt::variant::type::ostream,
            //xlnt::variant::type::storage,
            //xlnt::variant::type::ostorage,
            //xlnt::variant::type::vstream,
            //xlnt::variant::type::clsid,
        };
    }

    void construct_throw(const std::unordered_set<xlnt::variant::type> &allowed_types)
    {
        for (xlnt::variant::type type : get_all_types())
        {
            if (allowed_types.find(type) == allowed_types.end())
            {
                xlnt_assert_throws(xlnt::variant{type}, xlnt::invalid_parameter);
            }
            else
            {
                xlnt_assert_throws_nothing(xlnt::variant{type});
            }
        }
    }

    template<typename T>
    void construct_throw(const T &value, const std::unordered_set<xlnt::variant::type> &allowed_types)
    {
        for (xlnt::variant::type type : get_all_types())
        {
            if (allowed_types.find(type) == allowed_types.end())
            {
                xlnt_assert_throws(xlnt::variant(value, type), xlnt::invalid_parameter);
            }
            else
            {
                xlnt_assert_throws_nothing(xlnt::variant(value, type));
            }
        }
    }

    void test_construct_with_invalid_type()
    {
        construct_throw({xlnt::variant::type::null, xlnt::variant::type::empty});
        construct_throw<std::int32_t>(10, {xlnt::variant::type::i4, xlnt::variant::type::integer});
        construct_throw<std::uint32_t>(10, {xlnt::variant::type::ui4, xlnt::variant::type::uint});
        construct_throw<std::string>("test", {xlnt::variant::type::lpstr, xlnt::variant::type::lpwstr, xlnt::variant::type::bstr});
        construct_throw<xlnt::datetime>(xlnt::datetime(2025, 8, 15, 17, 30, 45, 123456), {xlnt::variant::type::date, xlnt::variant::type::filetime});
    }

    template<typename T>
    void construct_vector_throw(const std::vector<T> &value, const std::unordered_set<xlnt::variant::base_type> &allowed_types)
    {
        std::vector<xlnt::variant::base_type> all_base_types {
            xlnt::variant::base_type::variant,
            xlnt::variant::base_type::i1,
            xlnt::variant::base_type::i2,
            xlnt::variant::base_type::i4,
            xlnt::variant::base_type::i8,        // only for vectors
            //xlnt::variant::base_type::integer, // only for arrays
            xlnt::variant::base_type::ui1,
            xlnt::variant::base_type::ui2,
            xlnt::variant::base_type::ui4,
            xlnt::variant::base_type::ui8,       // only for vectors
            //xlnt::variant::base_type::uint,    // only for arrays
            xlnt::variant::base_type::r4,
            xlnt::variant::base_type::r8,
            //xlnt::variant::base_type::decimal, // only for arrays
            xlnt::variant::base_type::lpstr,     // only for vectors
            xlnt::variant::base_type::lpwstr,    // only for vectors
            xlnt::variant::base_type::bstr,
            xlnt::variant::base_type::date,
            xlnt::variant::base_type::filetime,  // only for vectors
            xlnt::variant::base_type::boolean,
            //xlnt::variant::base_type::cy,
            //xlnt::variant::base_type::error,
            //xlnt::variant::base_type::clsid,    // only for vectors
        };

        for (xlnt::variant::base_type type : all_base_types)
        {
            if (allowed_types.find(type) == allowed_types.end())
            {
                xlnt_assert_throws(xlnt::variant(value, type), xlnt::invalid_parameter);
            }
            else
            {
                xlnt_assert_throws_nothing(xlnt::variant(value, type));
            }
        }
    }

    void test_construct_vector_with_invalid_type()
    {
        construct_vector_throw<std::string>(
            {"test1", "test2", "test3", "test4"},
            {xlnt::variant::base_type::lpstr, xlnt::variant::base_type::lpwstr, xlnt::variant::base_type::bstr}
        );
        construct_vector_throw<xlnt::datetime>(
            {xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)},
            {xlnt::variant::base_type::date, xlnt::variant::base_type::filetime}
        );
    }
};
static variant_test_suite x;
