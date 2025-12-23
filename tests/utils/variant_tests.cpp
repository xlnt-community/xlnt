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

#include <xlnt/utils/variant.hpp>
#include <xlnt/utils/datetime.hpp>
#include <helpers/test_suite.hpp>

class variant_test_suite : public test_suite
{
public:
    variant_test_suite()
        : test_suite()
    {
        register_test(test_null);
        register_test(test_int32);
        register_test(test_int32_vector_from_initializer_list);
        register_test(test_int32_vector_from_vector);
        register_test(test_c_string);
        register_test(test_c_string_vector_from_initializer_list);
        register_test(test_c_string_vector_from_vector);
        register_test(test_std_string);
        register_test(test_std_string_vector_from_initializer_list);
        register_test(test_std_string_vector_from_vector);
        register_test(test_datetime);
        register_test(test_datetime_vector_from_initializer_list);
        register_test(test_datetime_vector_from_vector);
        register_test(test_bool);
        register_test(test_bool_vector_from_initializer_list);
        register_test(test_bool_vector_from_vector);
        register_test(test_variant_vector_from_initializer_list);
        register_test(test_variant_vector_from_vector);
    }

    template<typename T>
    void test_throw(const xlnt::variant &var_vec)
    {
        if (!std::is_same<T, bool>())
        {
            xlnt_assert_throws(var_vec.get<bool>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::int32_t>())
        {
            xlnt_assert_throws(var_vec.get<std::int32_t>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::string>())
        {
            xlnt_assert_throws(var_vec.get<std::string>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, xlnt::datetime>())
        {
            xlnt_assert_throws(var_vec.get<xlnt::datetime>(), xlnt::bad_variant_access);
        }

        // An additional check is unfortunately necessary for vectors because all vectors are currently
        // represented by variants internally (unfortunately), so we cannot reliably check that it throws.
        if (!std::is_same<T, std::vector<xlnt::variant>>() && !var_vec.is(xlnt::variant::type::vector))
        {
            xlnt_assert_throws(var_vec.get<std::vector<xlnt::variant>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<bool>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<bool>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::int32_t>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::int32_t>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<std::string>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<std::string>>(), xlnt::bad_variant_access);
        }

        if (!std::is_same<T, std::vector<xlnt::datetime>>())
        {
            xlnt_assert_throws(var_vec.get<std::vector<xlnt::datetime>>(), xlnt::bad_variant_access);
        }
    }

    template<typename T>
    void test_variant_vector(const xlnt::variant &var_vec, const std::vector<T>& vec)
    {
        xlnt_assert_equals(var_vec.value_type(), xlnt::variant::type::vector);
        xlnt_assert(var_vec.is(xlnt::variant::type::vector));
        xlnt_assert_equals(var_vec.get<std::vector<T>>(), vec);
        test_throw<std::vector<T>>(var_vec);
    }

    template<typename T>
    void test_vector_from_initializer_list(const std::initializer_list<T>& list)
    {
        xlnt::variant var_vec(list);
        test_variant_vector<T>(var_vec, std::vector<T>(list));
    }

    template<typename T>
    void test_vector_from_vector(const std::vector<T>& vec)
    {
        xlnt::variant var_vec(vec);
        test_variant_vector<T>(var_vec, vec);
    }

    void test_null()
    {
        xlnt::variant var_null;
        xlnt_assert_equals(var_null.value_type(), xlnt::variant::type::null);
        xlnt_assert(var_null.is(xlnt::variant::type::null));
        test_throw<void>(var_null); // null has no actual type
    }

    void test_int32()
    {
        constexpr std::int32_t value = 10;
        xlnt::variant var_int(value);
        xlnt_assert_equals(var_int.value_type(), xlnt::variant::type::i4);
        xlnt_assert(var_int.is(xlnt::variant::type::i4));
        xlnt_assert_equals(value, var_int.get<std::int32_t>());
        test_throw<std::int32_t>(var_int);
    }

    void test_int32_vector_from_initializer_list()
    {
        test_vector_from_initializer_list(std::initializer_list<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()});
    }

    void test_int32_vector_from_vector()
    {
        test_vector_from_vector(std::vector<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()});
    }

    void test_c_string()
    {
        xlnt::variant var_c_str("test1");
        xlnt_assert_equals(var_c_str.value_type(), xlnt::variant::type::lpstr);
        xlnt_assert(var_c_str.is(xlnt::variant::type::lpstr));
        xlnt_assert_equals("test1", var_c_str.get<std::string>());
        test_throw<std::string>(var_c_str);
    }

    void test_c_string_vector_from_initializer_list()
    {
        std::initializer_list<const char *> list{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(list);
        xlnt_assert_equals(var_vec.value_type(), xlnt::variant::type::vector);
        xlnt_assert(var_vec.is(xlnt::variant::type::vector));
        std::vector<std::string> vec_str;
        vec_str.reserve(list.size());
        for (const auto &elem : list)
        {
            vec_str.emplace_back(elem);
        }
        xlnt_assert_equals(var_vec.get<std::vector<std::string>>(), vec_str);
        test_throw<std::vector<std::string>>(var_vec);
    }

    void test_c_string_vector_from_vector()
    {
        std::vector<const char *> vec{"test1", "test2", "test3", "test4"};
        xlnt::variant var_vec(vec);
        xlnt_assert_equals(var_vec.value_type(), xlnt::variant::type::vector);
        xlnt_assert(var_vec.is(xlnt::variant::type::vector));
        std::vector<std::string> vec_str;
        vec_str.reserve(vec.size());
        for (const auto &elem : vec)
        {
            vec_str.emplace_back(elem);
        }
        xlnt_assert_equals(var_vec.get<std::vector<std::string>>(), vec_str);
        test_throw<std::vector<std::string>>(var_vec);
    }

    void test_std_string()
    {
        xlnt::variant var_std_str(std::string("test2"));
        xlnt_assert_equals(var_std_str.value_type(), xlnt::variant::type::lpstr);
        xlnt_assert(var_std_str.is(xlnt::variant::type::lpstr));
        xlnt_assert_equals("test2", var_std_str.get<std::string>());
        test_throw<std::string>(var_std_str);
    }

    void test_std_string_vector_from_initializer_list()
    {
        test_vector_from_initializer_list<std::string>(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"});
    }

    void test_std_string_vector_from_vector()
    {
        test_vector_from_vector<std::string>(std::vector<std::string>{"test1", "test2", "test3", "test4"});
    }

    void test_datetime()
    {
        const xlnt::datetime datetime(2025, 8, 15, 17, 30, 45, 123456);
        xlnt::variant var_datetime(datetime);
        xlnt_assert_equals(var_datetime.value_type(), xlnt::variant::type::date);
        xlnt_assert(var_datetime.is(xlnt::variant::type::date));
        xlnt_assert_equals(datetime, var_datetime.get<xlnt::datetime>());
        test_throw<xlnt::datetime>(var_datetime);
    }

    void test_datetime_vector_from_initializer_list()
    {
        test_vector_from_initializer_list<xlnt::datetime>(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)});
    }

    void test_datetime_vector_from_vector()
    {
        test_vector_from_vector<xlnt::datetime>(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)});
    }

    void test_bool()
    {
        xlnt::variant var_bool(true);
        xlnt_assert_equals(var_bool.value_type(), xlnt::variant::type::boolean);
        xlnt_assert(var_bool.is(xlnt::variant::type::boolean));
        xlnt_assert_equals(true, var_bool.get<bool>());
        test_throw<bool>(var_bool);
    }

    void test_bool_vector_from_initializer_list()
    {
        test_vector_from_initializer_list<bool>(std::initializer_list<bool>{true, false, false, true, true});
    }

    void test_bool_vector_from_vector()
    {
        test_vector_from_vector<bool>(std::vector<bool>{true, false, false, true, true});
    }

    void test_variant_vector_from_initializer_list()
    {
        // Test combinations of all types
        test_vector_from_initializer_list<xlnt::variant>(std::initializer_list<xlnt::variant>{
            // Test single elements for all types
            xlnt::variant(),
            xlnt::variant(std::int32_t(10)),
            xlnt::variant("test1"),
            xlnt::variant(std::string("test1")),
            xlnt::variant(xlnt::datetime(1975, 2, 14)),
            xlnt::variant(true),

            // Test initializer lists for all types
            xlnt::variant(std::initializer_list<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}),
            xlnt::variant(std::initializer_list<bool>{true, false, false, true, true}),

            // Test vectors for all types
            xlnt::variant(std::vector<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}),
            xlnt::variant(std::vector<bool>{true, false, false, true, true})
        });
    }

    void test_variant_vector_from_vector()
    {
        // Test combinations of all types
        test_vector_from_vector<xlnt::variant>(std::vector<xlnt::variant>{
            // Test single elements for all types
            xlnt::variant(),
            xlnt::variant(std::int32_t(10)),
            xlnt::variant("test1"),
            xlnt::variant(std::string("test1")),
            xlnt::variant(xlnt::datetime(1975, 2, 14)),
            xlnt::variant(true),

            // Test initializer lists for all types
            xlnt::variant(std::initializer_list<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::initializer_list<const char *>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::initializer_list<std::string>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::initializer_list<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}),
            xlnt::variant(std::initializer_list<bool>{true, false, false, true, true}),

            // Test vectors for all types
            xlnt::variant(std::vector<std::int32_t>{10, 20, 30, 40, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max()}),
            xlnt::variant(std::vector<const char *>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::vector<std::string>{"test1", "test2", "test3", "test4"}),
            xlnt::variant(std::vector<xlnt::datetime>{xlnt::datetime(1975, 2, 14), xlnt::datetime(2007, 12, 24), xlnt::datetime(2019, 3, 7)}),
            xlnt::variant(std::vector<bool>{true, false, false, true, true})
        });
    }
};
static variant_test_suite x;
