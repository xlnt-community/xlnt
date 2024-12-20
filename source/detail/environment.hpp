// Copyright (c) 2024 xlnt-community
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

// If available, allow using C++20 feature test macros for precise feature testing. Useful for compilers
// that partially implement certain features.
#ifdef __has_include
# if __has_include(<version>)
#   include <version>
# endif
#endif

// Unfortunately, the macro __cplusplus does not report the correct version under Visual Studio unless /Zc:__cplusplus is used during compilation.
// Source: https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-170
// In order to have proper feature testing for C++ features that don't have feature test macros, but also to avoid forcing others
// to configure their compiler properly, we'll need to define our own macro.
#ifndef XLNT_CPP_VERSION
  #if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus // take the larger one of the two (Microsoft does the same in the MSVC STL)
    #define XLNT_CPP_VERSION _MSVC_LANG
  #else
    #define XLNT_CPP_VERSION __cplusplus
  #endif
#endif

#define XLNT_CPP_11 201103L
#define XLNT_CPP_14 201402L
#define XLNT_CPP_17 201703L
#define XLNT_CPP_20 202002L
#define XLNT_CPP_23 202302L
// TODO: when C++26 is out, please update the C version check below as well!
//#define XLNT_CPP_26 TODO_VERSION

/// <summary>
/// Returns whether the C++ version `version` is supported by the current build configuration.
/// </summary>
/// <seealso cref="XLNT_CPP_11">
/// <seealso cref="XLNT_CPP_14">
/// <seealso cref="XLNT_CPP_17">
/// <seealso cref="XLNT_CPP_20">
/// <seealso cref="XLNT_CPP_23">
#define XLNT_HAS_CPP_VERSION(version) (1/version == 1/version && XLNT_CPP_VERSION >= version)

// Note: the first check ensures that a compiler partially implementing C++17 but implementing std::to_chars
// would be detected correctly, as long as the C++20 feature test macros are implemented. The second check
// ensures that a fully implemented C++17 compiler would be detected as well.
#if __cpp_lib_to_chars >= 201611L || XLNT_HAS_CPP_VERSION(XLNT_CPP_17)
  #define XLNT_DETAIL_FEATURE_TO_CHARS 1
#else
  #define XLNT_DETAIL_FEATURE_TO_CHARS -1
#endif


#define XLNT_C_99 199901L
#define XLNT_C_11 201112L
#define XLNT_C_17 201710L
#define XLNT_C_23 202311L

/// <summary>
/// Returns whether the C version `version` is supported by the current build configuration.
/// Unfortunately, while __STDC_VERSION__ must be defined in C compilers, when using C++ compilers it is
/// implementation-defined whether the __STDC_VERSION__ is defined or not. However, the C++ standard defines
/// for each C++ version which C version it refers to. This can be found out by using the following list:
/// C++98 -> C95
/// C++11 -> C99
/// C++17 -> C11
/// C++20 -> C17
/// C++26 -> C23
/// </summary>
/// <seealso cref="XLNT_C_11">
/// <seealso cref="XLNT_C_17">
/// <seealso cref="XLNT_C_23">
#define XLNT_HAS_C_VERSION(version) (1/version == 1/version && \
    ((__STDC_VERSION__ >= version) || \
     (XLNT_C_99 >= version && XLNT_HAS_CPP_VERSION(XLNT_CPP_11)) || \
     (XLNT_C_11 >= version && XLNT_HAS_CPP_VERSION(XLNT_CPP_17)) || \
     (XLNT_C_17 >= version && XLNT_HAS_CPP_VERSION(XLNT_CPP_20)) /*|| \
     (CLNT_C_23 >= version && XLNT_HAS_CPP_VERSION(XLNT_CPP_26))*/ \
     ))
      
    

// If you get a division by zero error, you probably misspelled the feature name.
// Developer note: XLNT_DETAIL_FEATURE_##feature should be set to
//    1: if feature is supported
//    -1: if the feature is not supported
/// <summary>
/// Returns whether the `feature` is supported by the current build configuration.
/// </summary>
/// Currently, the following features could be tested:
///  - TO_CHARS: returns whether compliant std::from_chars and std::to_chars implementations are available
#define XLNT_HAS_FEATURE(feature) (1/XLNT_DETAIL_FEATURE_##feature == 1)