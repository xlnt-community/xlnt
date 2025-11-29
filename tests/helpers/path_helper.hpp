#pragma once

#include <fstream>
#include <string>

#include <xlnt/utils/exceptions.hpp>
#include <xlnt/utils/path.hpp>
#include <xlnt/internal/features.hpp>

#define XLNT_TEST_LSTRING_LITERAL2(a) L##a
#define XLNT_TEST_U8STRING_LITERAL2(a) u8##a

#define XLNT_TEST_LSTRING_LITERAL(a) XLNT_TEST_LSTRING_LITERAL2(a)
#define XLNT_TEST_U8STRING_LITERAL(a) XLNT_TEST_U8STRING_LITERAL2(a)

#ifdef __cpp_char8_t
/// Casts const char8_t arrays from C++20 to const char arrays.
inline const char * to_char_ptr(const char8_t *utf8)
{
  return reinterpret_cast<const char *>(utf8);
}

/// Casts char8_t arrays from C++20 to char arrays.
inline char * to_char_ptr(char8_t *utf8)
{
  return reinterpret_cast<char *>(utf8);
}
#endif

// Casts a UTF-8 string literal to a narrow string literal without changing its encoding.
#ifdef __cpp_char8_t
// For C++20 and newer, interpret as UTF-8 and then cast to string literal
#define XLNT_TEST_U8_TO_CHAR_PTR(a) to_char_ptr(a)
#else
// For C++11, C++14 and C++17, simply interpret as UTF-8, which works with classic string literals.
#define XLNT_TEST_U8_TO_CHAR_PTR(a) a
#endif

// The following weird cast ensures that the string is UTF-8 encoded at all costs!
#define XLNT_TEST_ENSURE_UTF8_LITERAL(a) XLNT_TEST_U8_TO_CHAR_PTR(XLNT_TEST_U8STRING_LITERAL(a))

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
