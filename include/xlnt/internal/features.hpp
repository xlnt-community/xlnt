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

#pragma once

#include <xlnt/utils/environment.hpp>

// Checks that the specified C++ attribute is supported.
// Note: this only works in C++20 and newer - see https://en.cppreference.com/cpp/feature_test#Attributes
#ifdef __has_cpp_attribute
    #define XLNT_HAS_CPP_ATTRIBUTE(ATTRIBUTE) __has_cpp_attribute(ATTRIBUTE)
#else
    #define XLNT_HAS_CPP_ATTRIBUTE(ATTRIBUTE) 0
#endif

// Checks whether the specified C attribute is supported.
// Note: this only works in C23 and newer - see https://en.cppreference.com/c/language/attributes#Attribute_testing
#ifdef __has_c_attribute
    #define XLNT_HAS_C_ATTRIBUTE(ATTRIBUTE) __has_c_attribute(ATTRIBUTE)
#else
    #define XLNT_HAS_C_ATTRIBUTE(ATTRIBUTE) 0
#endif

// Checks whether the specified C/C++ attribute is supported.
// Supported in GCC and Clang (but otherwise not standardized). See https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005fattribute.html
#ifdef __has_attribute
    #define XLNT_HAS_ATTRIBUTE(ATTRIBUTE) __has_attribute(ATTRIBUTE)
#else
    #define XLNT_HAS_ATTRIBUTE(ATTRIBUTE) 0
#endif

// [[maybe_unused]] is supported in C++17 and newer, and C23 and newer
// C++17 needs an additional check as __has_cpp_attribute is only available with C++20 or higher (see above).
#if XLNT_HAS_CPP_ATTRIBUTE(maybe_unused) || XLNT_HAS_C_ATTRIBUTE(maybe_unused) || XLNT_HAS_CPP_VERSION(XLNT_CPP_17)
    #define XLNT_UNUSED [[maybe_unused]]
// [[gnu::unused]] is supported in GCC and Clang since C++11. See:
// -> https://gcc.gnu.org/onlinedocs/gcc/Common-Attributes.html#index-unused
// -> https://clang.llvm.org/docs/LanguageExtensions.html#non-standard-c-11-attributes
//
// Note: [[...]] syntax is only supported by C++11 and newer, and C23 and newer.
#elif XLNT_HAS_CPP_ATTRIBUTE(gnu::unused) || XLNT_HAS_C_ATTRIBUTE(gnu::unused)
    #define XLNT_UNUSED [[gnu::unused]]
// __attribute__((unused)) is supported in GCC and Clang. Can be useful with older compilers
// and in C code when compiled with a C version older than C23.
#elif XLNT_HAS_ATTRIBUTE(unused)
    #define XLNT_UNUSED __attribute__((unused))
#else
    #define XLNT_UNUSED
#endif


// Header detection helper. Available beginning with C++17.
#ifdef __has_include
  #define XLNT_HAS_INCLUDE(HEADER_NAME) __has_include(HEADER_NAME)
#else
  #define XLNT_HAS_INCLUDE(HEADER_NAME) 0
#endif

// If available, allow using C++20 feature test macros for precise feature testing. Useful for compilers
// that partially implement certain features.
#if XLNT_HAS_INCLUDE(<version>)
  #include <version>
#endif

// If you get a division by zero error, you probably misspelled the feature name.
// Developer note: XLNT_DETAIL_FEATURE_##feature should be set to
//    1: if feature is supported
//    -1: if the feature is not supported
/// <summary>
/// Returns whether the `feature` is supported by the current build configuration.
/// </summary>
/// Currently, the following features could be tested:
///  - TO_CHARS: returns whether compliant std::from_chars and std::to_chars implementations are available
///  - STRING_VIEW: returns whether compliant std::string_view, std::wstring_view, std::u16string_view and
///                 std::u32string_view implementations are available (note: std::u8string_view is part
///                 of C++20 - see U8_STRING_VIEW below)
///  - FILESYSTEM: returns whether a compliant std::filesystem implementations is available
///  - U8_STRING_VIEW: returns whether a compliant std::u8string_view implementation is available
#define XLNT_HAS_FEATURE(feature) (1/XLNT_DETAIL_FEATURE_##feature == 1)

// Note: the first check ensures that a compiler partially implementing C++17 but implementing std::to_chars
// would be detected correctly, as long as the C++20 feature test macros are implemented. The second check
// ensures that a fully implemented C++17 compiler would be detected as well.
#if defined(__cpp_lib_to_chars) || XLNT_HAS_CPP_VERSION(XLNT_CPP_17)
  #define XLNT_DETAIL_FEATURE_TO_CHARS 1
#else
  #define XLNT_DETAIL_FEATURE_TO_CHARS -1
#endif

#if defined(__cpp_lib_string_view) || XLNT_HAS_CPP_VERSION(XLNT_CPP_17)
  #define XLNT_DETAIL_FEATURE_STRING_VIEW 1
#else
  #define XLNT_DETAIL_FEATURE_STRING_VIEW -1
#endif

#if defined(__cpp_lib_filesystem) || XLNT_HAS_CPP_VERSION(XLNT_CPP_17)
  #define XLNT_DETAIL_FEATURE_FILESYSTEM 1
#else
  #define XLNT_DETAIL_FEATURE_FILESYSTEM -1
#endif

#if XLNT_HAS_FEATURE(STRING_VIEW) && defined(__cpp_lib_char8_t)
  #define XLNT_DETAIL_FEATURE_U8_STRING_VIEW 1
#else
  #define XLNT_DETAIL_FEATURE_U8_STRING_VIEW -1
#endif
