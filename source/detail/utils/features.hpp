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

#include <xlnt/utils/environment.hpp>

#if XLNT_HAS_CPP_VERSION(XLNT_CPP_17)
  #define XLNT_DETAIL_FEATURE_TO_CHARS 1
#else
  #define XLNT_DETAIL_FEATURE_TO_CHARS -1
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
#define XLNT_HAS_FEATURE(feature) (1/XLNT_DETAIL_FEATURE_##feature == 1)
