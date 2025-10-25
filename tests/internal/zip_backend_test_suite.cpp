// Copyright (c) 2025 xlnt-community
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

#include <xlnt/xlnt.hpp>
#include <helpers/test_suite.hpp>
#include <helpers/temporary_file.hpp>
#include <detail/serialization/archive_factory.hpp>
#include <sstream>
#include <vector>

/// Test suite to verify ZIP backend functionality
/// Tests both builtin and minizip-ng backends to ensure consistency
class zip_backend_test_suite : public test_suite
{
public:
    zip_backend_test_suite()
    {
        register_test(test_basic_read_write);
        register_test(test_multiple_files);
        register_test(test_file_listing);
        register_test(test_file_existence);
        register_test(test_path_normalization);
        register_test(test_concurrent_entry_protection);
        register_test(test_workbook_round_trip);
        register_test(test_large_file_support);
    }

    /// Test basic read/write operations
    void test_basic_read_write()
    {
        // Create a simple archive with one file
        std::stringstream archive_stream;

        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);
            auto entry = writer->open(xlnt::path("test.txt"));
            std::ostream stream(entry.get());
            stream << "Hello, World!";
        }

        // Read back the content
        archive_stream.seekg(0);
        auto reader = xlnt::detail::make_archive_reader(archive_stream);

        xlnt_assert(reader->has_file(xlnt::path("test.txt")));
        auto content = reader->read(xlnt::path("test.txt"));
        xlnt_assert_equals(content, std::string("Hello, World!"));
    }

    /// Test multiple files in archive
    void test_multiple_files()
    {
        std::stringstream archive_stream;

        // Write multiple files
        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);

            {
                auto entry1 = writer->open(xlnt::path("file1.txt"));
                std::ostream stream1(entry1.get());
                stream1 << "Content 1";
            }

            {
                auto entry2 = writer->open(xlnt::path("file2.txt"));
                std::ostream stream2(entry2.get());
                stream2 << "Content 2";
            }

            {
                auto entry3 = writer->open(xlnt::path("dir/file3.txt"));
                std::ostream stream3(entry3.get());
                stream3 << "Content 3";
            }
        }

        // Read back and verify
        archive_stream.seekg(0);
        auto reader = xlnt::detail::make_archive_reader(archive_stream);

        xlnt_assert(reader->has_file(xlnt::path("file1.txt")));
        xlnt_assert(reader->has_file(xlnt::path("file2.txt")));
        xlnt_assert(reader->has_file(xlnt::path("dir/file3.txt")));

        xlnt_assert_equals(reader->read(xlnt::path("file1.txt")), std::string("Content 1"));
        xlnt_assert_equals(reader->read(xlnt::path("file2.txt")), std::string("Content 2"));
        xlnt_assert_equals(reader->read(xlnt::path("dir/file3.txt")), std::string("Content 3"));
    }

    /// Test file listing functionality
    void test_file_listing()
    {
        std::stringstream archive_stream;

        // Create archive with known files
        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);

            auto entry1 = writer->open(xlnt::path("alpha.txt"));
            std::ostream(entry1.get()) << "A";
            entry1.reset();

            auto entry2 = writer->open(xlnt::path("beta.txt"));
            std::ostream(entry2.get()) << "B";
            entry2.reset();

            auto entry3 = writer->open(xlnt::path("gamma.txt"));
            std::ostream(entry3.get()) << "C";
        }

        // List files and verify
        archive_stream.seekg(0);
        auto reader = xlnt::detail::make_archive_reader(archive_stream);
        auto files = reader->files();

        xlnt_assert_equals(files.size(), static_cast<std::size_t>(3));

        // Verify all expected files are present
        bool has_alpha = false, has_beta = false, has_gamma = false;
        for (const auto &file : files) {
            if (file.string() == "alpha.txt") has_alpha = true;
            if (file.string() == "beta.txt") has_beta = true;
            if (file.string() == "gamma.txt") has_gamma = true;
        }

        xlnt_assert(has_alpha);
        xlnt_assert(has_beta);
        xlnt_assert(has_gamma);
    }

    /// Test file existence checks
    void test_file_existence()
    {
        std::stringstream archive_stream;

        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);
            auto entry = writer->open(xlnt::path("exists.txt"));
            std::ostream(entry.get()) << "I exist!";
        }

        archive_stream.seekg(0);
        auto reader = xlnt::detail::make_archive_reader(archive_stream);

        xlnt_assert(reader->has_file(xlnt::path("exists.txt")));
        xlnt_assert(!reader->has_file(xlnt::path("does_not_exist.txt")));
    }

    /// Test path normalization (Windows vs POSIX)
    void test_path_normalization()
    {
        std::stringstream archive_stream;

        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);
            auto entry = writer->open(xlnt::path("dir/subdir/file.txt"));
            std::ostream(entry.get()) << "Nested file";
        }

        archive_stream.seekg(0);
        auto reader = xlnt::detail::make_archive_reader(archive_stream);

        // Should be accessible with forward slashes
        xlnt_assert(reader->has_file(xlnt::path("dir/subdir/file.txt")));

        // Test content read
        auto content = reader->read(xlnt::path("dir/subdir/file.txt"));
        xlnt_assert_equals(content, std::string("Nested file"));
    }

    /// Test concurrent entry protection (should throw if trying to open multiple entries)
    void test_concurrent_entry_protection()
    {
        std::stringstream archive_stream;

        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);
            auto entry1 = writer->open(xlnt::path("file1.txt"));

            // Attempting to open another entry while first is still open should throw
            bool threw = false;
            try {
                auto entry2 = writer->open(xlnt::path("file2.txt"));
            } catch (const xlnt::exception &) {
                threw = true;
            }

            xlnt_assert(threw);
        }
    }

    /// Test full workbook round-trip
    void test_workbook_round_trip()
    {
        // Save to a fixed location for inspection
        xlnt::path test_file("test_output_workbook.xlsx");

        // Create a workbook with data
        xlnt::workbook wb_write;
        auto ws = wb_write.active_sheet();
        ws.cell("A1").value("Test Value");
        ws.cell("B2").value(42);
        ws.cell("C3").value(3.14159);
        ws.cell("D4").value("ZIP Backend Test");
        ws.cell("E5").value(true);

        // Save to file
        wb_write.save(test_file);

        // Load from file
        xlnt::workbook wb_read;
        wb_read.load(test_file);

        // Verify content
        auto ws_read = wb_read.active_sheet();
        xlnt_assert_equals(ws_read.cell("A1").value<std::string>(), std::string("Test Value"));
        xlnt_assert_equals(ws_read.cell("B2").value<int>(), 42);
        xlnt_assert_delta(ws_read.cell("C3").value<double>(), 3.14159, 0.00001);
        xlnt_assert_equals(ws_read.cell("D4").value<std::string>(), std::string("ZIP Backend Test"));
        xlnt_assert_equals(ws_read.cell("E5").value<bool>(), true);

        // Note: File is NOT deleted - saved as test_output_workbook.xlsx for inspection
    }

    /// Test large file support (basic stress test)
    void test_large_file_support()
    {
        std::stringstream archive_stream;

        // Create a large content string (1MB)
        std::string large_content(1024 * 1024, 'X');

        {
            auto writer = xlnt::detail::make_archive_writer(archive_stream);
            auto entry = writer->open(xlnt::path("large_file.txt"));
            std::ostream stream(entry.get());
            stream << large_content;
        }

        // Read back and verify size
        archive_stream.seekg(0);
        auto reader = xlnt::detail::make_archive_reader(archive_stream);

        auto read_content = reader->read(xlnt::path("large_file.txt"));
        xlnt_assert_equals(read_content.size(), large_content.size());
        xlnt_assert_equals(read_content, large_content);

        // Also save to file for inspection
        std::ofstream out_file("test_output_archive.zip", std::ios::binary);
        out_file << archive_stream.rdbuf();
        out_file.close();

        // Note: File is NOT deleted - saved as test_output_archive.zip for inspection
    }
};

static zip_backend_test_suite x;
