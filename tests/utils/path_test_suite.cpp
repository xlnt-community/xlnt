// Copyright (c) 2014-2022 Thomas Fussell
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

#include <xlnt/utils/path.hpp>
#include <xlnt/utils/string_helpers.hpp>
#include <xlnt/internal/features.hpp>
#include <helpers/path_helper.hpp>
#include <helpers/temporary_file.hpp>
#include <helpers/test_suite.hpp>

class path_test_suite : public test_suite
{
public:
    path_test_suite()
    {
        register_test(test_exists);
#ifdef _MSC_VER
        register_test(test_msvc_empty_path_wide);
#endif
        register_test(test_append);
        register_test(test_parent);
        register_test(test_relative_to);
#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
        register_test(test_append_u8);
#endif
    }

    void test_exists()
    {
        temporary_file temp;

        if (temp.get_path().exists())
        {
            path_helper::delete_file(temp.get_path());
        }

        xlnt_assert(!temp.get_path().exists());
        std::ofstream stream(temp.get_path().string());
        xlnt_assert(temp.get_path().exists());
    }

#ifdef _MSC_VER
    void test_msvc_empty_path_wide()
    {
        xlnt::path empty_path;
        std::wstring path_wide;
        xlnt_assert_throws_nothing(path_wide = empty_path.wstring());
        xlnt_assert(path_wide.empty());
    }
#endif

    void test_append()
    {
        xlnt::path path("hello");
        path = path.append("world");
        xlnt_assert_equals(path.string(), "hello/world");
    }

    void test_parent()
    {
        xlnt_assert_equals(xlnt::path().parent().string(), "");
        xlnt_assert_equals(xlnt::path("xl").parent().string(), "");
        xlnt_assert_equals(xlnt::path("xl/drawings").parent().string(), "xl");
        xlnt_assert_equals(xlnt::path("/").parent().string(), "/");
        xlnt_assert_equals(xlnt::path("/xl").parent().string(), "/");
        xlnt_assert_equals(xlnt::path("/xl/drawings").parent().string(), "/xl");
#ifdef WIN32
        xlnt_assert_equals(xlnt::path("C:\\").parent().string(), "C:\\");
        xlnt_assert_equals(xlnt::path("C:\\xl").parent().string(), "C:\\");
        xlnt_assert_equals(xlnt::path("C:\\xl\\drawings").parent().string(), "C:\\xl");
        xlnt_assert_equals(xlnt::path("C:/xl").parent().string(), "C:/");
#endif
    }

    void test_relative_to()
    {
        const auto image = xlnt::path("/xl/media/image1.bmp");

        const auto drawings = xlnt::path("/xl/drawings");
        const auto relative_image = image.relative_to(drawings);
        const auto root = xlnt::path("/");
        const auto relative_from_root = image.relative_to(root);
        const auto xl = xlnt::path("/xl");
        const auto relative_to_ancestor = xl.relative_to(drawings);
        const auto relative_to_self = xl.relative_to(xl);

        xlnt_assert_equals(relative_image.string(), "../media/image1.bmp");
        xlnt_assert_equals(relative_image.resolve(drawings).string(), image.string());
        xlnt_assert_equals(relative_from_root.string(), "xl/media/image1.bmp");
        xlnt_assert_equals(relative_from_root.resolve(root).string(), image.string());
        xlnt_assert_equals(relative_to_ancestor.string(), "..");
        xlnt_assert_equals(relative_to_ancestor.resolve(drawings).string(), xl.string());
        xlnt_assert_equals(xlnt::path("../..").resolve(drawings).string(), "/");
        xlnt_assert_equals(relative_to_self.string(), "");
        xlnt_assert_equals(relative_to_self.resolve(xl).string(), xl.string());

        // A relative path cannot be computed across different roots.
        xlnt_assert_equals(image.relative_to(xlnt::path("relative/base")).string(), image.string());
#ifdef WIN32
        const auto windows_base = xlnt::path("C:\\xl\\drawings");
        const auto windows_target = xlnt::path("C:\\xl\\media\\image1.bmp");
        const auto windows_relative = windows_target.relative_to(windows_base);
        const auto other_drive = xlnt::path("D:\\media\\image1.bmp");

        xlnt_assert_equals(windows_relative.string(), "../media/image1.bmp");
        xlnt_assert_equals(windows_relative.resolve(windows_base).string(), windows_target.string());
        xlnt_assert_equals(other_drive.relative_to(xlnt::path("C:\\drawings")).string(), other_drive.string());
#endif
    }

#if XLNT_HAS_FEATURE(U8_STRING_VIEW)
    void test_append_u8()
    {
        xlnt::path path(u8"🤔🥳😇");
        path = path.append(u8"🍕🍟🍔");
        xlnt_assert_equals(path.string(), xlnt::to_char_ptr(u8"🤔🥳😇/🍕🍟🍔"));
    }
#endif

};
static path_test_suite x;
