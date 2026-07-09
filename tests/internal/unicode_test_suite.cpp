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

#include <xlnt/utils/string_helpers.hpp>

#include <detail/unicode.hpp>

#include <helpers/test_suite.hpp>

#define UNICODE_TEST_STRING "🤔🥳😇"

class unicode_test_suite : public test_suite
{
public:
    unicode_test_suite()
    {
        register_test(test_convert_utf8_to_utf16);
#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
        register_test(test_convert_utf8_u8_to_utf16);
#endif
        register_test(test_convert_utf8_to_utf32);
#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
        register_test(test_convert_utf8_u8_to_utf32);
#endif
        register_test(test_convert_utf16_to_utf8);
#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
        register_test(test_convert_utf16_to_utf8_u8);
#endif
        register_test(test_convert_utf32_to_utf8);
#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
        register_test(test_convert_utf32_to_utf8_u8);
#endif
    }

    void test_convert_utf8_to_utf16()
    {
        const char *utf8 = XLNT_U8(UNICODE_TEST_STRING);
        std::u16string result = xlnt::detail::utf8_to_utf16(utf8);
        xlnt_assert_equals(result, XLNT_DETAIL_U16STRING_LITERAL(UNICODE_TEST_STRING));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    void test_convert_utf8_u8_to_utf16()
    {
        const char8_t *utf8 = XLNT_DETAIL_U8STRING_LITERAL(UNICODE_TEST_STRING);
        std::u16string result = xlnt::detail::utf8_to_utf16(utf8);
        xlnt_assert_equals(result, XLNT_DETAIL_U16STRING_LITERAL(UNICODE_TEST_STRING));
    }
#endif

    void test_convert_utf8_to_utf32()
    {
        const char *utf8 = XLNT_U8(UNICODE_TEST_STRING);
        std::u32string result = xlnt::detail::utf8_to_utf32(utf8);
        xlnt_assert_equals(result, XLNT_DETAIL_U32STRING_LITERAL(UNICODE_TEST_STRING));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    void test_convert_utf8_u8_to_utf32()
    {
        const char8_t *utf8 = XLNT_DETAIL_U8STRING_LITERAL(UNICODE_TEST_STRING);
        std::u32string result = xlnt::detail::utf8_to_utf32(utf8);
        xlnt_assert_equals(result, XLNT_DETAIL_U32STRING_LITERAL(UNICODE_TEST_STRING));
    }
#endif

    void test_convert_utf16_to_utf8()
    {
        const char16_t *utf16 = XLNT_DETAIL_U16STRING_LITERAL(UNICODE_TEST_STRING);
        std::string result = xlnt::detail::utf16_to_utf8(utf16);
        xlnt_assert_equals(result, XLNT_U8(UNICODE_TEST_STRING));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    void test_convert_utf16_to_utf8_u8()
    {
        const char16_t *utf16 = XLNT_DETAIL_U16STRING_LITERAL(UNICODE_TEST_STRING);
        std::u8string result = xlnt::detail::utf16_to_utf8_u8(utf16);
        xlnt_assert_equals(result, XLNT_DETAIL_U8STRING_LITERAL(UNICODE_TEST_STRING));
    }
#endif

    void test_convert_utf32_to_utf8()
    {
        const char32_t *utf32 = XLNT_DETAIL_U32STRING_LITERAL(UNICODE_TEST_STRING);
        std::string result = xlnt::detail::utf32_to_utf8(utf32);
        xlnt_assert_equals(result, XLNT_U8(UNICODE_TEST_STRING));
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    void test_convert_utf32_to_utf8_u8()
    {
        const char32_t *utf32 = XLNT_DETAIL_U32STRING_LITERAL(UNICODE_TEST_STRING);
        std::u8string result = xlnt::detail::utf32_to_utf8_u8(utf32);
        xlnt_assert_equals(result, XLNT_DETAIL_U8STRING_LITERAL(UNICODE_TEST_STRING));
    }
#endif
};
static unicode_test_suite x;
