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

#include <detail/binary.hpp>
#include <helpers/test_suite.hpp>

class binary_test_suite : public test_suite
{
  public:
    binary_test_suite()
    {
        register_test(test_reader_read_pointer_valid_char_to_char);
        register_test(test_reader_read_pointer_invalid_char_to_char);
        register_test(test_reader_read_pointer_valid_char_to_int);
        register_test(test_reader_read_pointer_invalid_char_to_int);
        register_test(test_reader_as_vector_empty);
        register_test(test_reader_as_vector_valid_char_to_char);
        register_test(test_reader_as_vector_valid_char_to_int);
        register_test(test_reader_as_vector_valid_int_to_char);
        register_test(test_reader_as_vector_invalid_char_to_int);
        register_test(test_reader_read_vector_empty);
        register_test(test_reader_read_vector_valid_char_to_char);
        register_test(test_reader_read_vector_valid_char_to_int);
        register_test(test_reader_read_vector_int_to_char);
        register_test(test_reader_read_vector_invalid_char_to_int);
        register_test(test_reader_read_vector_past_end);
        register_test(test_writer_write_valid_char_to_char);
        register_test(test_writer_write_valid_int_to_char);
        register_test(test_writer_write_invalid_offset);
        register_test(test_writer_assign_vector_empty);
        register_test(test_writer_assign_vector_valid_char_to_char);
        register_test(test_writer_assign_vector_char_to_int);
        register_test(test_writer_assign_vector_valid_int_to_char);
        register_test(test_writer_assign_vector_invalid_char_to_int);
        register_test(test_writer_assign_string_empty);
        register_test(test_writer_assign_string_valid_narrow_string_to_char);
        register_test(test_writer_assign_string_valid_wide_string_to_wchar);
#if XLNT_HAS_FEATURE(U8_STRING)
        register_test(test_writer_assign_string_valid_u8string_to_char8);
#endif
        register_test(test_writer_assign_string_valid_u16string_to_char16);
        register_test(test_writer_assign_string_valid_u32string_to_char32);
        register_test(test_writer_assign_string_valid_u16string_to_char);
        register_test(test_writer_append_vector_empty);
        register_test(test_writer_append_vector_valid_char_to_char);
        register_test(test_writer_append_vector_char_to_int);
        register_test(test_writer_append_vector_valid_int_to_char);
        register_test(test_writer_append_vector_invalid_char_to_int);
        register_test(test_writer_append_vector_invalid_offset);
        register_test(test_writer_append_reader_empty);
        register_test(test_writer_append_reader_valid_char_to_char);
        register_test(test_writer_append_reader_char_to_int);
        register_test(test_writer_append_reader_valid_int_to_char);
        register_test(test_writer_append_reader_invalid_char_to_int);
        register_test(test_writer_append_reader_invalid_writer_offset);
        register_test(test_writer_append_reader_invalid_reader_offset);
        register_test(test_read_valid_single_element_from_stream);
        register_test(test_read_invalid_single_element_from_stream);
        register_test(test_read_valid_vector_from_stream);
        register_test(test_read_empty_vector_from_stream);
        register_test(test_read_invalid_vector_from_stream);
        register_test(test_read_valid_string_without_NUL_from_stream);
        register_test(test_read_valid_string_with_NUL_from_stream);
        register_test(test_read_empty_string_from_stream);
        register_test(test_read_invalid_string_from_stream);
    }

    void test_reader_read_pointer_valid_char_to_char()
    {
        std::vector<char> container(1);
        xlnt::detail::binary_reader<char> reader(container);

        xlnt_assert_equals(reader.offset(), 0);
        const char* ptr = nullptr;
        xlnt_assert_throws_nothing(ptr = reader.read_pointer<char>());
        xlnt_assert_equals(container.data(), ptr);
        xlnt_assert_equals(reader.offset(), sizeof(char));
    }

    void test_reader_read_pointer_invalid_char_to_char()
    {
        std::vector<char> container(sizeof(char) - 1);
        xlnt::detail::binary_reader<char> reader(container);

        // Cannot read less than 1 char.
        xlnt_assert_throws(reader.read_pointer<char>(), xlnt::invalid_parameter);
    }

    void test_reader_read_pointer_valid_char_to_int()
    {
        std::vector<char> container(sizeof(int));
        xlnt::detail::binary_reader<char> reader(container);

        xlnt_assert_equals(reader.offset(), 0);
        const int* ptr = nullptr;
        xlnt_assert_throws_nothing(ptr = reader.read_pointer<int>());
        xlnt_assert_equals(container.data(), reinterpret_cast<const char *>(ptr));
        xlnt_assert_equals(reader.offset(), sizeof(int));
    }

    void test_reader_read_pointer_invalid_char_to_int()
    {
        std::vector<char> container(sizeof(int) - 1);
        xlnt::detail::binary_reader<char> reader(container);

        // Cannot read less than 1 int.
        xlnt_assert_throws(reader.read_pointer<int>(), xlnt::invalid_parameter);
    }

    void test_reader_as_vector_empty()
    {
        std::vector<char> container;
        xlnt::detail::binary_reader<char> reader(container);

        std::vector<char> copy;
        xlnt_assert_throws_nothing(copy = reader.as_vector<char>());
        xlnt_assert_equals(container, copy);
    }

    void test_reader_as_vector_valid_char_to_char()
    {
        std::vector<char> container(1);
        xlnt::detail::binary_reader<char> reader(container);

        std::vector<char> copy;
        xlnt_assert_throws_nothing(copy = reader.as_vector<char>());
        xlnt_assert_equals(container, copy);
    }

    void test_reader_as_vector_valid_char_to_int()
    {
        std::vector<char> container(sizeof(int));
        xlnt::detail::binary_reader<char> reader(container);

        std::vector<int> copy;
        xlnt_assert_throws_nothing(copy = reader.as_vector<int>());
        xlnt_assert_equals(container.size(), copy.size() * sizeof(int));
    }

    void test_reader_as_vector_valid_int_to_char()
    {
        std::vector<int> container(1);
        xlnt::detail::binary_reader<int> reader(container);

        std::vector<char> copy;
        xlnt_assert_throws_nothing(copy = reader.as_vector<char>());
        xlnt_assert_equals(container.size() * sizeof(int), copy.size());
    }

    void test_reader_as_vector_invalid_char_to_int()
    {
        std::vector<char> container(sizeof(int) - 1);
        xlnt::detail::binary_reader<char> reader(container);

        xlnt_assert_throws(reader.as_vector<int>(), xlnt::invalid_parameter);
    }

    void test_reader_read_vector_empty()
    {
        std::vector<char> container;
        xlnt::detail::binary_reader<char> reader(container);

        std::vector<char> copy;
        xlnt_assert_throws_nothing(copy = reader.read_vector<char>(0));
        xlnt_assert_equals(container, copy);
        xlnt_assert_throws(reader.read_vector<char>(1), xlnt::invalid_parameter);
    }

    void test_reader_read_vector_valid_char_to_char()
    {
        std::vector<char> container(1);
        xlnt::detail::binary_reader<char> reader(container);

        std::vector<char> copy;
        xlnt_assert_throws_nothing(copy = reader.read_vector<char>(1));
        xlnt_assert_equals(container, copy);
        xlnt_assert_throws(copy = reader.read_vector<char>(1), xlnt::invalid_parameter);
    }

    void test_reader_read_vector_valid_char_to_int()
    {
        std::vector<char> container(sizeof(int));
        xlnt::detail::binary_reader<char> reader(container);

        std::vector<int> copy;
        xlnt_assert_throws_nothing(copy = reader.read_vector<int>(1));
        xlnt_assert_equals(container.size(), copy.size() * sizeof(int));
    }

    void test_reader_read_vector_int_to_char()
    {
        constexpr std::size_t COUNT = 2;
        std::vector<int> container(COUNT);
        xlnt::detail::binary_reader<int> reader(container);

        // Cannot read a size that does not contain the exact same
        // number of bytes as the size of the element.
        std::vector<char> copy;
        for (std::size_t n = 0; n < COUNT * sizeof(int) + 1; ++n)
        {
            xlnt_assert_equals(reader.offset(), 0);

            if (n % sizeof(int) == 0)
            {
                xlnt_assert_throws_nothing(copy = reader.read_vector<char>(n));
                xlnt_assert_equals(n, copy.size());
                reader.offset(0);
            }
            else
            {
                xlnt_assert_throws(reader.read_vector<char>(n), xlnt::invalid_parameter);
            }
        }
    }

    void test_reader_read_vector_invalid_char_to_int()
    {
        std::vector<char> container(sizeof(int) - 1);
        xlnt::detail::binary_reader<char> reader(container);

        xlnt_assert_throws(reader.read_vector<int>(sizeof(int) - 1), xlnt::invalid_parameter);
    }

    void test_reader_read_vector_past_end()
    {
        std::vector<int> container(1);
        xlnt::detail::binary_reader<int> reader(container);
        xlnt_assert_throws(reader.read_vector<int>(2), xlnt::invalid_parameter);
    }

    void test_writer_write_valid_char_to_char()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_equals(writer.offset(), 0);
        xlnt_assert_throws_nothing(writer.write<char>('h'));
        xlnt_assert_equals(container.size(), sizeof(char));
        xlnt_assert_equals(writer.count(), sizeof(char));
        xlnt_assert_equals(writer.offset(), sizeof(char));
        xlnt_assert_equals(container.at(0), 'h');
    }

    void test_writer_write_valid_int_to_char()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_equals(writer.offset(), 0);
        xlnt_assert_throws_nothing(writer.write<int>(1234));
        xlnt_assert_equals(container.size(), sizeof(int));
        xlnt_assert_equals(writer.count(), sizeof(int));
        xlnt_assert_equals(writer.offset(), sizeof(int));
    }

    void test_writer_write_invalid_offset()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_equals(writer.offset(), 0);
        writer.offset(1);
        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_equals(writer.offset(), 1);
        // Cannot write on invalid offset of binary_writer.
        xlnt_assert_throws(writer.write<char>(0), xlnt::invalid_attribute);
        xlnt_assert_equals(container.size(), 0);
        writer.reset();

        xlnt_assert_throws_nothing(writer.write<char>(0));
        xlnt_assert_equals(container.size(), sizeof(char));
        xlnt_assert_equals(writer.count(), sizeof(char));
        xlnt_assert_equals(writer.offset(), sizeof(char));
        container.pop_back();
        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_equals(writer.offset(), sizeof(char));
        // Cannot write on invalid offset of binary_writer.
        xlnt_assert_throws(writer.write<char>(0), xlnt::invalid_attribute);
    }

    void test_writer_assign_vector_empty()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char>(std::vector<char>{}));
        xlnt_assert_equals(container.size(), 0);
        xlnt_assert_equals(writer.count(), 0);
    }

    void test_writer_assign_vector_valid_char_to_char()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char>(std::vector<char>{'h', 'i'}));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), 'h');
        xlnt_assert_equals(container.at(1), 'i');
    }

    void test_writer_assign_vector_char_to_int()
    {
        std::vector<int> container;
        xlnt::detail::binary_writer<int> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        constexpr std::size_t COUNT = 2;
        std::vector<char> vec;
        vec.reserve(COUNT * sizeof(int) + 1);

        // Cannot assign a size that does not contain the exact same
        // number of bytes as the size of the element.
        for (std::size_t n = 0; n < COUNT * sizeof(int) + 1; ++n)
        {
            if (n % sizeof(int) == 0)
            {
                xlnt_assert_throws_nothing(writer.assign<char>(vec));
                const std::size_t num_ints = n / sizeof(int);
                xlnt_assert_equals(container.size(), num_ints);
                xlnt_assert_equals(writer.count(), num_ints);
            }
            else
            {
                xlnt_assert_throws(writer.assign<char>(vec), xlnt::invalid_parameter);
            }

            vec.push_back(n);
        }
    }

    void test_writer_assign_vector_valid_int_to_char()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<int>(std::vector<int>{1234}));
        xlnt_assert_equals(container.size(), sizeof(int));
        xlnt_assert_equals(writer.count(), sizeof(int));
    }

    void test_writer_assign_vector_invalid_char_to_int()
    {
        std::vector<int> container;
        xlnt::detail::binary_writer<int> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        // Cannot assign a size that does not contain the exact same
        // number of bytes as the size of the element.
        xlnt_assert_throws(writer.assign<char>(std::vector<char>{0}), xlnt::invalid_parameter);
        xlnt_assert_equals(container.size(), 0);
        xlnt_assert_equals(writer.count(), 0);
    }

    void test_writer_assign_string_empty()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char>(std::string{}, true));
        xlnt_assert_equals(container.size(), 1);
        xlnt_assert_equals(writer.count(), 1);
        xlnt_assert_equals(container.at(0), '\0');

        xlnt_assert_throws_nothing(writer.assign<char>(std::string{}, false));
        xlnt_assert_equals(container.size(), 0);
        xlnt_assert_equals(writer.count(), 0);
    }

    void test_writer_assign_string_valid_narrow_string_to_char()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char>(std::string{"hi"}, true));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(container.at(0), 'h');
        xlnt_assert_equals(container.at(1), 'i');
        xlnt_assert_equals(container.at(2), '\0');

        xlnt_assert_throws_nothing(writer.assign<char>(std::string{"hi"}, false));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), 'h');
        xlnt_assert_equals(container.at(1), 'i');
    }

    void test_writer_assign_string_valid_wide_string_to_wchar()
    {
        std::vector<wchar_t> container;
        xlnt::detail::binary_writer<wchar_t> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<wchar_t>(std::wstring{L"hi"}, true));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(container.at(0), L'h');
        xlnt_assert_equals(container.at(1), L'i');
        xlnt_assert_equals(container.at(2), L'\0');

        xlnt_assert_throws_nothing(writer.assign<wchar_t>(std::wstring{L"hi"}, false));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), L'h');
        xlnt_assert_equals(container.at(1), L'i');
    }

#if XLNT_HAS_FEATURE(U8_STRING)
    void test_writer_assign_string_valid_u8string_to_char8()
    {
        std::vector<char8_t> container;
        xlnt::detail::binary_writer<char8_t> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char8_t>(std::u8string{u8"hi"}, true));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(container.at(0), u8'h');
        xlnt_assert_equals(container.at(1), u8'i');
        xlnt_assert_equals(container.at(2), u8'\0');

        xlnt_assert_throws_nothing(writer.assign<char8_t>(std::u8string{u8"hi"}, false));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), u8'h');
        xlnt_assert_equals(container.at(1), u8'i');
    }
#endif

    void test_writer_assign_string_valid_u16string_to_char16()
    {
        std::vector<char16_t> container;
        xlnt::detail::binary_writer<char16_t> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char16_t>(std::u16string{u"hi"}, true));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(container.at(0), u'h');
        xlnt_assert_equals(container.at(1), u'i');
        xlnt_assert_equals(container.at(2), u'\0');

        xlnt_assert_throws_nothing(writer.assign<char16_t>(std::u16string{u"hi"}, false));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), u'h');
        xlnt_assert_equals(container.at(1), u'i');
    }

    void test_writer_assign_string_valid_u32string_to_char32()
    {
        std::vector<char32_t> container;
        xlnt::detail::binary_writer<char32_t> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char32_t>(std::u32string{U"hi"}, true));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(container.at(0), U'h');
        xlnt_assert_equals(container.at(1), U'i');
        xlnt_assert_equals(container.at(2), U'\0');

        xlnt_assert_throws_nothing(writer.assign<char32_t>(std::u32string{U"hi"}, false));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), U'h');
        xlnt_assert_equals(container.at(1), U'i');
    }

    void test_writer_assign_string_valid_u16string_to_char()
    {
        std::vector<char> container;
        xlnt::detail::binary_writer<char> writer(container);

        xlnt_assert_equals(writer.count(), 0);
        xlnt_assert_throws_nothing(writer.assign<char16_t>(std::u16string{u"hi"}, true));
        xlnt_assert_equals(container.size(), 3 * sizeof(char16_t));
        xlnt_assert_equals(writer.count(), 3 * sizeof(char16_t));

        xlnt_assert_throws_nothing(writer.assign<char16_t>(std::u16string{u"hi"}, false));
        xlnt_assert_equals(container.size(), 2 * sizeof(char16_t));
        xlnt_assert_equals(writer.count(), 2 * sizeof(char16_t));
    }

    void test_writer_append_vector_empty()
    {
        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents (shouldn't happen here anyway)

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_throws_nothing(writer.append<char>(std::vector<char>{}));
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(container.at(0), 'h');
        xlnt_assert_equals(container.at(1), 'i');
    }

    void test_writer_append_vector_valid_char_to_char()
    {
        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_throws_nothing(writer.append<char>(std::vector<char>{' ', 'X', 'L', 'N', 'T'}));
        xlnt_assert_equals(container.size(), 7);
        xlnt_assert_equals(writer.count(), 7);
        xlnt_assert_equals(container.at(0), 'h');
        xlnt_assert_equals(container.at(1), 'i');
        xlnt_assert_equals(container.at(2), ' ');
        xlnt_assert_equals(container.at(3), 'X');
        xlnt_assert_equals(container.at(4), 'L');
        xlnt_assert_equals(container.at(5), 'N');
        xlnt_assert_equals(container.at(6), 'T');
    }

    void test_writer_append_vector_char_to_int()
    {
        std::vector<int> container{0, 1};
        xlnt::detail::binary_writer<int> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        constexpr std::size_t COUNT = 2;
        std::vector<char> vec;
        vec.reserve(COUNT * sizeof(int) + 1);

        // Cannot append a size that does not contain the exact same
        // number of bytes as the size of the element.
        for (std::size_t n = 0; n < COUNT * sizeof(int) + 1; ++n)
        {
            if (n % sizeof(int) == 0)
            {
                xlnt_assert_throws_nothing(writer.append<char>(vec));
                const std::size_t num_ints = n / sizeof(int);
                xlnt_assert_equals(container.size(), num_ints + 2);
                xlnt_assert_equals(writer.count(), num_ints + 2);
                writer.offset(2);
            }
            else
            {
                xlnt_assert_throws(writer.append<char>(vec), xlnt::invalid_parameter);
            }

            vec.push_back(n);
        }
    }

    void test_writer_append_vector_valid_int_to_char()
    {
        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_throws_nothing(writer.append<int>(std::vector<int>{1234}));
        xlnt_assert_equals(container.size(), sizeof(int) + 2);
        xlnt_assert_equals(writer.count(), sizeof(int) + 2);
    }

    void test_writer_append_vector_invalid_char_to_int()
    {
        std::vector<int> container{0, 1};
        xlnt::detail::binary_writer<int> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        // Cannot append a size that does not contain the exact same
        // number of bytes as the size of the element.
        xlnt_assert_throws(writer.append<char>(std::vector<char>{0}), xlnt::invalid_parameter);
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
    }

    void test_writer_append_vector_invalid_offset()
    {
        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(writer.offset(), 2);
        writer.offset(3);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(writer.offset(), 3);
        // Cannot append to an invalid offset of binary_writer.
        xlnt_assert_throws(writer.append<char>(std::vector<char>{0}), xlnt::invalid_attribute);
        xlnt_assert_equals(container.size(), 2);
        writer.offset(2);

        xlnt_assert_throws_nothing(writer.append<char>(std::vector<char>{0}));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(writer.offset(), 3);
        container.pop_back();
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(writer.offset(), 3);
        // Cannot append to an invalid offset of binary_writer.
        xlnt_assert_throws(writer.append<char>(std::vector<char>{0}), xlnt::invalid_attribute);
    }

    void test_writer_append_reader_empty()
    {
        std::vector<char> container_read;
        xlnt::detail::binary_reader<char> reader(container_read);

        std::vector<char> container_write{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container_write);
        writer.offset(2); // don't overwrite current contents (shouldn't happen here anyway)

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_throws_nothing(writer.append<char>(reader, 0));
        xlnt_assert_equals(container_write.size(), 2);
        xlnt_assert_equals(writer.count(), 2);

        xlnt_assert_throws(writer.append<char>(reader, 1), xlnt::invalid_parameter);
        xlnt_assert_equals(container_write.size(), 2);
        xlnt_assert_equals(writer.count(), 2);

        xlnt_assert_equals(container_write.at(0), 'h');
        xlnt_assert_equals(container_write.at(1), 'i');
    }

    void test_writer_append_reader_valid_char_to_char()
    {
        std::vector<char> container_read{' ', 'X', 'L', 'N', 'T'};
        xlnt::detail::binary_reader<char> reader(container_read);

        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_throws_nothing(writer.append<char>(reader, 5));
        xlnt_assert_equals(container.size(), 7);
        xlnt_assert_equals(writer.count(), 7);
        xlnt_assert_equals(container.at(0), 'h');
        xlnt_assert_equals(container.at(1), 'i');
        xlnt_assert_equals(container.at(2), ' ');
        xlnt_assert_equals(container.at(3), 'X');
        xlnt_assert_equals(container.at(4), 'L');
        xlnt_assert_equals(container.at(5), 'N');
        xlnt_assert_equals(container.at(6), 'T');
    }

    void test_writer_append_reader_char_to_int()
    {
        std::vector<int> container_write{0, 1};
        xlnt::detail::binary_writer<int> writer(container_write);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        constexpr std::size_t COUNT = 2;
        std::vector<char> container_read;
        container_read.reserve(COUNT * sizeof(int) + 1);
        xlnt::detail::binary_reader<char> reader(container_read);

        // Cannot append a size that does not contain the exact same
        // number of bytes as the size of the element.
        for (std::size_t n = 0; n < COUNT * sizeof(int) + 1; ++n)
        {
            if (n % sizeof(int) == 0)
            {
                xlnt_assert_throws_nothing(writer.append<char>(reader, n));
                const std::size_t num_ints = n / sizeof(int);
                xlnt_assert_equals(container_write.size(), num_ints + 2);
                xlnt_assert_equals(writer.count(), num_ints + 2);
                writer.offset(2);
            }
            else
            {
                xlnt_assert_throws(writer.append<char>(reader, n), xlnt::invalid_parameter);
            }

            container_read.push_back(n);
        }
    }

    void test_writer_append_reader_valid_int_to_char()
    {
        std::vector<int> container_read{0};
        xlnt::detail::binary_reader<int> reader(container_read);

        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_throws_nothing(writer.append<int>(reader, 1));
        xlnt_assert_equals(container.size(), sizeof(int) + 2);
        xlnt_assert_equals(writer.count(), sizeof(int) + 2);
    }

    void test_writer_append_reader_invalid_char_to_int()
    {
        std::vector<char> container_read{0};
        xlnt::detail::binary_reader<char> reader(container_read);

        std::vector<int> container{0, 1};
        xlnt::detail::binary_writer<int> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        // Cannot append a size that does not contain the exact same
        // number of bytes as the size of the element.
        xlnt_assert_throws(writer.append<char>(reader, 1), xlnt::invalid_parameter);
        xlnt_assert_equals(container.size(), 2);
        xlnt_assert_equals(writer.count(), 2);
    }

    void test_writer_append_reader_invalid_writer_offset()
    {
        std::vector<char> container_read{0};
        xlnt::detail::binary_reader<char> reader(container_read);

        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(writer.offset(), 2);
        writer.offset(3);
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(writer.offset(), 3);
        // Cannot append to an invalid offset of binary_writer.
        xlnt_assert_throws(writer.append<char>(reader, 1), xlnt::invalid_attribute);
        xlnt_assert_equals(container.size(), 2);
        writer.offset(2);

        xlnt_assert_throws_nothing(writer.append<char>(reader, 1));
        xlnt_assert_equals(container.size(), 3);
        xlnt_assert_equals(writer.count(), 3);
        xlnt_assert_equals(writer.offset(), 3);
        container.pop_back();
        xlnt_assert_equals(writer.count(), 2);
        xlnt_assert_equals(writer.offset(), 3);
        // Cannot append to an invalid offset of binary_writer.
        xlnt_assert_throws(writer.append<char>(reader, 1), xlnt::invalid_attribute);
    }

    void test_writer_append_reader_invalid_reader_offset()
    {
        std::vector<char> container_read{0};
        xlnt::detail::binary_reader<char> reader(container_read);

        std::vector<char> container{'h', 'i'};
        xlnt::detail::binary_writer<char> writer(container);
        writer.offset(2); // don't overwrite current contents

        xlnt_assert_equals(reader.count(), 1);
        xlnt_assert_equals(reader.offset(), 0);
        reader.offset(1);
        xlnt_assert_equals(reader.count(), 1);
        xlnt_assert_equals(reader.offset(), 1);
        // Cannot append from an invalid offset of binary_reader.
        xlnt_assert_throws(writer.append<char>(reader, 1), xlnt::invalid_parameter);
        xlnt_assert_equals(container.size(), 2);
        reader.offset(0);
        xlnt_assert_equals(reader.offset(), 0);
        // Cannot append an invalid number of elements of binary_reader.
        xlnt_assert_throws(writer.append<char>(reader, 2), xlnt::invalid_parameter);
    }

    void test_read_valid_single_element_from_stream()
    {
        std::stringstream ss;
        ss << 'h';
        char elem = '\0';
        xlnt_assert_throws_nothing(elem = xlnt::detail::read<char>(ss));
        xlnt_assert_equals(elem, 'h');
    }

    void test_read_invalid_single_element_from_stream()
    {
        std::stringstream ss;
        char elem = '\0';
        xlnt_assert_throws(elem = xlnt::detail::read<char>(ss), xlnt::invalid_parameter);
        xlnt_assert_equals(elem, '\0');
    }

    void test_read_valid_vector_from_stream()
    {
        std::stringstream ss;
        ss << "hi XLNT";
        std::vector<char> vec;
        xlnt_assert_throws_nothing(vec = xlnt::detail::read_vector<char>(ss, 7));
        xlnt_assert_equals(vec.at(0), 'h');
        xlnt_assert_equals(vec.at(1), 'i');
        xlnt_assert_equals(vec.at(2), ' ');
        xlnt_assert_equals(vec.at(3), 'X');
        xlnt_assert_equals(vec.at(4), 'L');
        xlnt_assert_equals(vec.at(5), 'N');
        xlnt_assert_equals(vec.at(6), 'T');
    }

    void test_read_empty_vector_from_stream()
    {
        std::stringstream ss;
        std::vector<char> vec;
        xlnt_assert_throws_nothing(vec = xlnt::detail::read_vector<char>(ss, 0));
    }

    void test_read_invalid_vector_from_stream()
    {
        std::stringstream ss;
        std::vector<char> vec;
        xlnt_assert_throws(vec = xlnt::detail::read_vector<char>(ss, 1), xlnt::invalid_parameter);
    }

    void test_read_valid_string_without_NUL_from_stream()
    {
        std::stringstream ss;
        ss << "hi XLNT";
        std::string str;
        xlnt_assert_throws_nothing(str = xlnt::detail::read_string<char>(ss, 7, false));
        xlnt_assert_equals(str, "hi XLNT");
    }

    void test_read_valid_string_with_NUL_from_stream()
    {
        std::stringstream ss;
        std::string_view str_with_NUL {"hi XLNT\0", 8};
        ss << str_with_NUL;
        std::string str;
        xlnt_assert_throws_nothing(str = xlnt::detail::read_string<char>(ss, 8, true));
        xlnt_assert_equals(str, "hi XLNT");
        ss.seekg(0);
        xlnt_assert_throws_nothing(str = xlnt::detail::read_string<char>(ss, 8, false));
        xlnt_assert_equals(str, str_with_NUL);
    }

    void test_read_empty_string_from_stream()
    {
        std::stringstream ss;
        std::string str;
        // Reading 0 elements must always work.
        xlnt_assert_throws_nothing(str = xlnt::detail::read_string<char>(ss, 0, true));
        // Re-reading 0 elements must also work.
        xlnt_assert_throws_nothing(str = xlnt::detail::read_string<char>(ss, 0, false));
    }

    void test_read_invalid_string_from_stream()
    {
        std::stringstream ss;
        std::string str;
        xlnt_assert_throws(str = xlnt::detail::read_string<char>(ss, 1, true), xlnt::invalid_parameter);
    }
};

static binary_test_suite x;
