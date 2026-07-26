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

#include <detail/cryptography/compound_document.hpp>
#include <helpers/test_suite.hpp>

class compound_document_test_suite : public test_suite
{
  public:
    compound_document_test_suite()
    {
        register_test(test_compound_document_entry_set_valid_name);
        register_test(test_compound_document_entry_set_invalid_name);
        register_test(test_expect_valid_sector_or_chain_end);
        register_test(test_expect_valid_entry_or_no_stream);
    }

    void test_compound_document_entry_set_valid_name()
    {
        xlnt::detail::compound_document_entry entry;
        entry.name("hello world");
        xlnt_assert_equals(entry.name(), "hello world");

        entry.name("🤔🥳😇");
        xlnt_assert_equals(entry.name(), "🤔🥳😇");
    }

    void test_compound_document_entry_set_invalid_name()
    {
        xlnt::detail::compound_document_entry entry;
        xlnt_assert_throws(entry.name("/"), xlnt::invalid_parameter);
        xlnt_assert_throws(entry.name("\\"), xlnt::invalid_parameter);
        xlnt_assert_throws(entry.name(":"), xlnt::invalid_parameter);
        xlnt_assert_throws(entry.name("!"), xlnt::invalid_parameter);
        xlnt_assert_throws(entry.name("Test with more than 31 characters"), xlnt::invalid_parameter);
        xlnt_assert(entry.name().empty());
    }

    void test_expect_valid_sector_or_chain_end()
    {
        xlnt_assert_throws_nothing(xlnt::detail::expect_valid_sector_or_chain_end(0));
        xlnt_assert_throws_nothing(xlnt::detail::expect_valid_sector_or_chain_end(xlnt::detail::MAXREGSECT));
        xlnt_assert_throws(xlnt::detail::expect_valid_sector_or_chain_end(0xFFFFFFFB), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::expect_valid_sector_or_chain_end(xlnt::detail::DIFSECT), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::expect_valid_sector_or_chain_end(xlnt::detail::FATSECT), xlnt::invalid_parameter);
        xlnt_assert_throws_nothing(xlnt::detail::expect_valid_sector_or_chain_end(xlnt::detail::ENDOFCHAIN));
        xlnt_assert_throws(xlnt::detail::expect_valid_sector_or_chain_end(xlnt::detail::FREESECT), xlnt::invalid_parameter);
    }

    void test_expect_valid_entry_or_no_stream()
    {
        xlnt_assert_throws_nothing(xlnt::detail::expect_valid_entry_or_no_stream(0));
        xlnt_assert_throws_nothing(xlnt::detail::expect_valid_entry_or_no_stream(xlnt::detail::MAXREGSID));
        xlnt_assert_throws(xlnt::detail::expect_valid_entry_or_no_stream(0xFFFFFFFB), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::expect_valid_entry_or_no_stream(0xFFFFFFFC), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::expect_valid_entry_or_no_stream(0xFFFFFFFD), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::expect_valid_entry_or_no_stream(0xFFFFFFFE), xlnt::invalid_parameter);
        xlnt_assert_throws_nothing(xlnt::detail::expect_valid_entry_or_no_stream(xlnt::detail::NOSTREAM));
    }
};

static compound_document_test_suite x;
