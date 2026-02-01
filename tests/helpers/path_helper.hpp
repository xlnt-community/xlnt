#pragma once

#include <fstream>
#include <string>

#include <xlnt/utils/exceptions.hpp>
#include <xlnt/utils/path.hpp>
#include <xlnt/utils/string_helpers.hpp>
#include <xlnt/internal/features.hpp>


#if XLNT_HAS_INCLUDE(<string_view>) && XLNT_HAS_FEATURE(U8_STRING_VIEW)
  #include <string_view>
#endif

#ifndef XLNT_TEST_DATA_DIR
#define XLNT_TEST_DATA_DIR ""
#endif

#ifndef XLNT_BENCHMARK_DATA_DIR
#define XLNT_BENCHMARK_DATA_DIR ""
#endif

#ifndef XLNT_SAMPLE_DATA_DIR
#define XLNT_SAMPLE_DATA_DIR ""
#endif

// Prepends the string literal prefix to the provided string literal.
// Useful when defining a string literal once, then using it with multiple string types.
#define XLNT_TEST_LSTRING_LITERAL2(a) L##a
#define XLNT_TEST_U8STRING_LITERAL2(a) u8##a
#define XLNT_TEST_U16STRING_LITERAL2(a) u##a
#define XLNT_TEST_U32STRING_LITERAL2(a) U##a
#define XLNT_TEST_LSTRING_LITERAL(a) XLNT_TEST_LSTRING_LITERAL2(a)
#define XLNT_TEST_U8STRING_LITERAL(a) XLNT_TEST_U8STRING_LITERAL2(a)
#define XLNT_TEST_U16STRING_LITERAL(a) XLNT_TEST_U16STRING_LITERAL2(a)
#define XLNT_TEST_U32STRING_LITERAL(a) XLNT_TEST_U32STRING_LITERAL2(a)

// Prepends the u8 string literal prefix to the provided string literal, then
// casts it to a narrow string literal without changing its encoding or performing any conversions.
// Useful when defining a string literal once, then using it with both narrow and u8 strings.
#define XLNT_TEST_ENSURE_UTF8_LITERAL(a) XLNT_U8_TO_CHAR_PTR(XLNT_TEST_U8STRING_LITERAL(a))


class path_helper
{
public:
    static xlnt::path test_data_directory()
    {
        static const std::string data_dir = XLNT_TEST_ENSURE_UTF8_LITERAL(XLNT_TEST_DATA_DIR);
        return xlnt::path(data_dir);
    }

    static xlnt::path test_file(const std::string &filename)
    {
        return test_data_directory().append(xlnt::path(filename));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    static xlnt::path test_file(std::u8string_view filename)
    {
        return test_data_directory().append(xlnt::path(filename));
    }
#endif

    static xlnt::path benchmark_data_directory()
    {
        static const std::string data_dir = XLNT_TEST_ENSURE_UTF8_LITERAL(XLNT_BENCHMARK_DATA_DIR);
        return xlnt::path(data_dir);
    }

    static xlnt::path benchmark_file(const std::string &filename)
    {
        return benchmark_data_directory().append(xlnt::path(filename));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    static xlnt::path benchmark_file(std::u8string_view filename)
    {
        return benchmark_data_directory().append(xlnt::path(filename));
    }
#endif

    static xlnt::path sample_data_directory()
    {
        static const std::string data_dir = XLNT_TEST_ENSURE_UTF8_LITERAL(XLNT_SAMPLE_DATA_DIR);
        return xlnt::path(data_dir);
    }

    static xlnt::path sample_file(const std::string &filename)
    {
        return sample_data_directory().append(xlnt::path(filename));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    static xlnt::path sample_file(std::u8string_view filename)
    {
        return sample_data_directory().append(xlnt::path(filename));
    }
#endif

    static void copy_file(const xlnt::path &source, const xlnt::path &destination, bool overwrite)
    {
        if(!overwrite && destination.exists())
        {
            throw xlnt::exception("destination file already exists and overwrite==false for file at path \"" + destination.string() + "\"");
        }

        std::ifstream src(source.string(), std::ios::binary);
        std::ofstream dst(destination.string(), std::ios::binary);

        dst << src.rdbuf();
    }

    static void delete_file(const xlnt::path &path)
    {
      std::remove(path.string().c_str());
    }
};
