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
        register_test(test_is_chain_end);
        register_test(test_has_invalid_start_sector);
        register_test(test_is_invalid_entry);
        register_test(test_check_header);
        register_test(test_check_header_version_4_remaining_part);
        register_test(test_check_unallocated_entry);
        register_test(test_check_non_unallocated_entry);
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

    void test_is_chain_end()
    {
        xlnt_assert(!xlnt::detail::is_chain_end(0));
        xlnt_assert(!xlnt::detail::is_chain_end(xlnt::detail::MAXREGSECT));
        xlnt_assert_throws(xlnt::detail::is_chain_end(0xFFFFFFFB), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::is_chain_end(xlnt::detail::DIFSECT), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::is_chain_end(xlnt::detail::FATSECT), xlnt::invalid_parameter);
        xlnt_assert(xlnt::detail::is_chain_end(xlnt::detail::ENDOFCHAIN));
        xlnt_assert_throws(xlnt::detail::is_chain_end(xlnt::detail::FREESECT), xlnt::invalid_parameter);
    }

    void test_has_invalid_start_sector()
    {
        xlnt::detail::compound_document_entry entry;
        entry.type = xlnt::detail::compound_document_entry::entry_type::Unallocated;
        xlnt_assert(xlnt::detail::has_invalid_start_sector(entry));
        entry.type = xlnt::detail::compound_document_entry::entry_type::Storage;
        xlnt_assert(xlnt::detail::has_invalid_start_sector(entry));
        entry.type = xlnt::detail::compound_document_entry::entry_type::LockBytes;
        xlnt_assert(xlnt::detail::has_invalid_start_sector(entry));
        entry.type = xlnt::detail::compound_document_entry::entry_type::Property;
        xlnt_assert(xlnt::detail::has_invalid_start_sector(entry));

        entry.type = xlnt::detail::compound_document_entry::entry_type::RootStorage;
        entry.start_sector = 0;
        xlnt_assert(!xlnt::detail::has_invalid_start_sector(entry));
        entry.start_sector = xlnt::detail::MAXREGSECT;
        xlnt_assert(!xlnt::detail::has_invalid_start_sector(entry));
        entry.start_sector = 0xFFFFFFFB;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
        entry.start_sector = xlnt::detail::DIFSECT;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
        entry.start_sector = xlnt::detail::FATSECT;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
        entry.start_sector = xlnt::detail::ENDOFCHAIN;
        xlnt_assert_throws_nothing(xlnt::detail::has_invalid_start_sector(entry));
        entry.start_sector = xlnt::detail::FREESECT;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);

        entry.type = xlnt::detail::compound_document_entry::entry_type::Stream;
        entry.start_sector = 0;
        xlnt_assert(!xlnt::detail::has_invalid_start_sector(entry));
        entry.start_sector = xlnt::detail::MAXREGSECT;
        xlnt_assert(!xlnt::detail::has_invalid_start_sector(entry));
        entry.start_sector = 0xFFFFFFFB;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
        entry.start_sector = xlnt::detail::DIFSECT;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
        entry.start_sector = xlnt::detail::FATSECT;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
        entry.start_sector = xlnt::detail::ENDOFCHAIN;
        xlnt_assert_throws_nothing(xlnt::detail::has_invalid_start_sector(entry));
        entry.start_sector = xlnt::detail::FREESECT;
        xlnt_assert_throws(xlnt::detail::has_invalid_start_sector(entry), xlnt::invalid_parameter);
    }

    void test_is_invalid_entry()
    {
        xlnt_assert(!xlnt::detail::is_invalid_entry(0));
        xlnt_assert(!xlnt::detail::is_invalid_entry(xlnt::detail::MAXREGSID));
        xlnt_assert_throws(xlnt::detail::is_invalid_entry(0xFFFFFFFB), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::is_invalid_entry(0xFFFFFFFC), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::is_invalid_entry(0xFFFFFFFD), xlnt::invalid_parameter);
        xlnt_assert_throws(xlnt::detail::is_invalid_entry(0xFFFFFFFE), xlnt::invalid_parameter);
        xlnt_assert(xlnt::detail::is_invalid_entry(xlnt::detail::NOSTREAM));
    }

    void test_check_header()
    {
        xlnt::detail::compound_document_header header;
        // Default-constructed header should never throw.
        xlnt_assert_throws_nothing(check_header(header));

        header.header_signature = 0;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.header_signature = 0xE11AB1A1E011CFD0;
        xlnt_assert_throws_nothing(check_header(header));

        header.header_clsid.at(0) = 1;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.header_clsid.at(0) = 0;
        xlnt_assert_throws_nothing(check_header(header));

        header.major_version = 2;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.major_version = 3;
        header.sector_shift = 0x0009; // necessary for version 3
        xlnt_assert_throws_nothing(check_header(header));
        header.major_version = 4;
        header.sector_shift = 0x000C; // necessary for version 4
        xlnt_assert_throws_nothing(check_header(header));
        header.major_version = 5;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.major_version = 3;
        header.sector_shift = 0x0009;

        header.byte_order = xlnt::detail::compound_document_header::byte_order_type::big_endian;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.byte_order = xlnt::detail::compound_document_header::byte_order_type::little_endian;
        xlnt_assert_throws_nothing(check_header(header));

        header.major_version = 3;
        header.sector_shift = 0x0008;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.sector_shift = 0x0009;
        xlnt_assert_throws_nothing(check_header(header));
        header.sector_shift = 0x000A;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.major_version = 4;
        header.sector_shift = 0x000B;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.sector_shift = 0x000C;
        xlnt_assert_throws_nothing(check_header(header));
        header.sector_shift = 0x000D;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.major_version = 3;
        header.sector_shift = 0x0009;

        header.mini_sector_shift = 0x0005;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.mini_sector_shift = 0x0006;
        xlnt_assert_throws_nothing(check_header(header));
        header.mini_sector_shift = 0x0007;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.mini_sector_shift = 0x0006;

        header.reserved.at(0) = 1;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.reserved.at(0) = 0;
        xlnt_assert_throws_nothing(check_header(header));

        header.major_version = 3;
        header.num_directory_sectors = 1;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.num_directory_sectors = 0;
        xlnt_assert_throws_nothing(check_header(header));

        header.mini_stream_cutoff_size = 0;
        xlnt_assert_throws(check_header(header), xlnt::invalid_file);
        header.mini_stream_cutoff_size = 0x00001000;
        xlnt_assert_throws_nothing(check_header(header));
    }

    void test_check_header_version_4_remaining_part()
    {
        std::array<std::uint8_t, 3584> remaining {{ 0 }};
        xlnt::detail::compound_document_header header;
        header.major_version = 3;
        xlnt_assert_throws(xlnt::detail::check_header_version_4_remaining_part(header, remaining), xlnt::invalid_parameter);
        header.major_version = 4;
        xlnt_assert_throws_nothing(xlnt::detail::check_header_version_4_remaining_part(header, remaining));
        remaining.at(0) = 1;
        xlnt_assert_throws(xlnt::detail::check_header_version_4_remaining_part(header, remaining), xlnt::invalid_file);
    }

    void test_check_unallocated_entry()
    {
        constexpr xlnt::detail::directory_id ID = 0;
        constexpr xlnt::detail::sector_id SECTOR = 0;
        xlnt::detail::compound_document_entry entry;
        // Default-constructed Unallocated entry should never throw.
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.type = xlnt::detail::compound_document_entry::entry_type::Storage;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_parameter);
        entry.type = xlnt::detail::compound_document_entry::entry_type::Unallocated;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        // NOTE: Some implementations seem to not initialize this buffer at all, so we cannot check it for correctness.
        /*entry.directory_entry_name.at(0) = u'a';
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name.at(0) = u'\0';
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));*/

        entry.directory_entry_name_length = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name_length = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.color = xlnt::detail::compound_document_entry::entry_color::Black;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.color = xlnt::detail::compound_document_entry::entry_color::Red;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.left_sibling = 0;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.left_sibling = xlnt::detail::NOSTREAM;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.right_sibling = 0;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.right_sibling = xlnt::detail::NOSTREAM;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.child = 0;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.child = xlnt::detail::NOSTREAM;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.clsid.at(0) = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.clsid.at(0) = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.state_bits = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.state_bits = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.creation_time = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        // NOTE: some implementations seem to use the timestamp 116444736000000000, which is 1970-01-01 00:00:00 UTC.
        entry.creation_time = 116444736000000000;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.creation_time = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.modified_time = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        // NOTE: some implementations seem to use the timestamp 116444736000000000, which is 1970-01-01 00:00:00 UTC.
        entry.modified_time = 116444736000000000;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.modified_time = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.start_sector = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        // According to the specification, it must be 0, but it seems that some immplementations
        // initialize it with ENDOFCHAIN or FREESECT, which is honestly not wrong either. So let's accept that.
        entry.start_sector = xlnt::detail::ENDOFCHAIN;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.start_sector = xlnt::detail::FREESECT;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
        entry.start_sector = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));

        entry.stream_size = 1;
        xlnt_assert_throws(check_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.stream_size = 0;
        xlnt_assert_throws_nothing(check_unallocated_entry(entry, ID, SECTOR));
    }

    void test_check_non_unallocated_entry()
    {
        constexpr xlnt::detail::directory_id ID = 0;
        constexpr xlnt::detail::sector_id SECTOR = 0;
        xlnt::detail::compound_document_entry entry;
        // Default-constructed Unallocated entry should always throw.
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_parameter);
        entry.type = xlnt::detail::compound_document_entry::entry_type::Storage;
        entry.name("hi");
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        entry.directory_entry_name_length = 0;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name_length = 1;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name_length = 2; // must throw, as this character is not \0
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name.at(0) = '\0';
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.directory_entry_name_length = 3;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name_length = 4; // must throw, as this character is not \0
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name.at(1) = '\0';
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.directory_entry_name_length = 64;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.directory_entry_name_length = 65;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.name("hi");
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        // We'll set the string manually, since calling entry.name() would throw too.
        entry.directory_entry_name_length = 4;
        entry.directory_entry_name.at(1) = u'\0';
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.directory_entry_name.at(0) = u'/';
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name.at(0) = u'\\';
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name.at(0) = u':';
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.directory_entry_name.at(0) = u'/';
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.name("hi");
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        entry.type = xlnt::detail::compound_document_entry::entry_type::Storage;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.type = xlnt::detail::compound_document_entry::entry_type::Stream;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.type = xlnt::detail::compound_document_entry::entry_type::LockBytes;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.type = xlnt::detail::compound_document_entry::entry_type::Property;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.type = xlnt::detail::compound_document_entry::entry_type::RootStorage;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        entry.color = static_cast<xlnt::detail::compound_document_entry::entry_color>(2);
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.color = xlnt::detail::compound_document_entry::entry_color::Red;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.color = xlnt::detail::compound_document_entry::entry_color::Black;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        entry.type = xlnt::detail::compound_document_entry::entry_type::Stream;
        entry.clsid.at(0) = 1;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.clsid.at(0) = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        // NOTE: unfortunately cannot be enforced, as some files:
        // - have a root entry with timestamp 116444736000000000, which is 1970-01-01 00:00:00 UTC
        // - have a stream with an actual timestamp
        /*entry.creation_time = 1;
        entry.type = xlnt::detail::compound_document_entry::entry_type::Stream;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.creation_time = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.creation_time = 1;
        entry.type = xlnt::detail::compound_document_entry::entry_type::RootStorage;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.creation_time = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));*/

        // NOTE: unfortunately cannot be enforced, as some files have a stream with an actual timestamp.
        /*entry.modified_time = 1;
        entry.type = xlnt::detail::compound_document_entry::entry_type::Stream;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.modified_time = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));*/

        entry.type = xlnt::detail::compound_document_entry::entry_type::Storage;
        entry.start_sector = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.start_sector = 1;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.start_sector = xlnt::detail::MAXREGSECT;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.start_sector = 0xFFFFFFFB;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.start_sector = xlnt::detail::DIFSECT;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.start_sector = xlnt::detail::FATSECT;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.start_sector = xlnt::detail::ENDOFCHAIN;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        // It seems that some immplementations initialize it with FREESECT,
        // which is honestly not wrong either. So let's accept that.
        entry.start_sector = xlnt::detail::FREESECT;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
        entry.start_sector = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));

        entry.type = xlnt::detail::compound_document_entry::entry_type::Storage;
        entry.stream_size = 1;
        xlnt_assert_throws(check_non_unallocated_entry(entry, ID, SECTOR), xlnt::invalid_file);
        entry.stream_size = 0;
        xlnt_assert_throws_nothing(check_non_unallocated_entry(entry, ID, SECTOR));
    }
};

static compound_document_test_suite x;
