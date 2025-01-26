#pragma once

#include <fstream>
#include <string>

// If available, allow using C++20 feature test macros for precise feature testing. Useful for compilers
// that partially implement certain features.
#ifdef __has_include
# if __has_include(<version>)
#   include <version>
# endif
#endif

#ifdef __has_include
# if __has_include(<string_view>)
#   include <string_view>
# endif
#endif

#include <xlnt/utils/exceptions.hpp>
#include <xlnt/utils/path.hpp>
#include <detail/utils/string_helpers.hpp>

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
    static xlnt::path test_data_directory(const std::string &append = "")
    {
        // Note: the following weird cast ensures that the string is UTF-8 encoded at all costs!
        static const std::string data_dir = U8_CAST_CONST_LITERAL(U8STRING_LITERAL(XLNT_TEST_DATA_DIR));
        return xlnt::path(data_dir);
    }

#ifdef __cpp_lib_char8_t
    static xlnt::path test_data_directory_u8(std::u8string_view append = u8"")
    {
        static constexpr std::u8string_view data_dir = U8STRING_LITERAL(XLNT_TEST_DATA_DIR);
        return xlnt::path(data_dir);
    }
#endif

    static xlnt::path test_file(const std::string &filename)
    {
        return test_data_directory().append(xlnt::path(filename));
    }

#ifdef __cpp_lib_char8_t
    static xlnt::path test_file(std::u8string_view filename)
    {
        return test_data_directory_u8().append(xlnt::path(filename));
    }
#endif

    static xlnt::path benchmark_data_directory(const std::string &append = "")
    {
        // Note: the following weird cast ensures that the string is UTF-8 encoded at all costs!
        static const std::string data_dir = U8_CAST_CONST_LITERAL(U8STRING_LITERAL(XLNT_BENCHMARK_DATA_DIR));
        return xlnt::path(data_dir);
    }

#ifdef __cpp_lib_char8_t
    static xlnt::path benchmark_data_directory_u8(std::u8string_view append = u8"")
    {
        static constexpr std::u8string_view data_dir = U8STRING_LITERAL(XLNT_BENCHMARK_DATA_DIR);
        return xlnt::path(data_dir);
    }
#endif

    static xlnt::path benchmark_file(const std::string &filename)
    {
        return benchmark_data_directory().append(xlnt::path(filename));
    }

#ifdef __cpp_lib_char8_t
    static xlnt::path benchmark_file(std::u8string_view filename)
    {
        return benchmark_data_directory_u8().append(xlnt::path(filename));
    }
#endif

    static xlnt::path sample_data_directory(const std::string &append = "")
    {
        // Note: the following weird cast ensures that the string is UTF-8 encoded at all costs!
        static const std::string data_dir = U8_CAST_CONST_LITERAL(U8STRING_LITERAL(XLNT_SAMPLE_DATA_DIR));
        return xlnt::path(data_dir);
    }

#ifdef __cpp_lib_char8_t
    static xlnt::path sample_data_directory_u8(std::u8string_view append = u8"")
    {
        static constexpr std::u8string_view data_dir = U8STRING_LITERAL(XLNT_SAMPLE_DATA_DIR);
        return xlnt::path(data_dir);
    }
#endif

    static xlnt::path sample_file(const std::string &filename)
    {
        return sample_data_directory().append(xlnt::path(filename));
    }

#ifdef __cpp_lib_char8_t
    static xlnt::path sample_file(std::u8string_view filename)
    {
        return sample_data_directory_u8().append(xlnt::path(filename));
    }
#endif

    static void copy_file(const xlnt::path &source, const xlnt::path &destination, bool overwrite)
    {
        if(!overwrite && destination.exists())
        {
            throw xlnt::exception("destination file already exists and overwrite==false");
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
