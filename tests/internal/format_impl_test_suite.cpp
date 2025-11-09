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

#include <detail/implementations/format_impl.hpp>
#include <detail/implementations/stylesheet.hpp>
#include <helpers/test_suite.hpp>
#include <xlnt/cell/cell.hpp>
#include <xlnt/internal/format_impl_ptr.hpp>
#include <xlnt/styles/fill.hpp>
#include <xlnt/styles/format.hpp>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/worksheet.hpp>


class format_impl_test_suite : public test_suite
{
  public:
    format_impl_test_suite()
    {
        register_test(test_format_impl_ptr);
        register_test(test_inplace_editing_non_shared_format);
        register_test(test_reference);
    }

    void test_format_impl_ptr()
    {
        xlnt::detail::format_impl_ptr p1(new xlnt::detail::format_impl());
        xlnt_assert_equals(p1.use_count(), 1);

        // copy constructor
        auto p2 = p1;
        xlnt_assert_equals(p1.use_count(), 2);

        p2 = nullptr;
        xlnt_assert_equals(p1.use_count(), 1);

        // self assignment
        p1 = p1;
        xlnt_assert_equals(p1.use_count(), 1);

        {
            // constructor
            xlnt::detail::format_impl_ptr p4(p1.get());
            xlnt_assert_equals(p1.use_count(), 2);
        }
        // destructor
        xlnt_assert_equals(p1.use_count(), 1);

        // move constructor
        auto p3 = std::move(p1);
        xlnt_assert_equals(p3.use_count(), 1);
    }

    void test_inplace_editing_non_shared_format()
    {
        xlnt::detail::stylesheet s;
        s.garbage_collection_enabled = false;

        xlnt_assert_equals(s.format_impls.size(), 0);

        xlnt::format f = [&s](const xlnt::format& f){
            xlnt_assert_equals(s.format_impls.size(), 1);
            return f;
        } (s.create_format(false)
            .border(xlnt::border())
            .fill(xlnt::fill(xlnt::pattern_fill()))
            .font(xlnt::font())
            .number_format(xlnt::number_format())
            .style("X")
          );

        xlnt_assert_equals(s.format_impls.size(), 1);
    }

    void test_reference()
    {
        xlnt::detail::references ref1;

        xlnt_assert_equals(ref1, 0);

        ++ref1;
        xlnt_assert_equals(ref1, 1);

        xlnt::detail::references ref2(ref1);
        xlnt_assert_equals(ref2, 0); // a copied object is a new object. The new object is not referenced (although it contains the same data as another object that may be referenced)

        xlnt::detail::references ref3;
        ref3 = ref2; // same, but for copy constructor;
        xlnt_assert_equals(ref3, 0);

        xlnt::detail::references ref4(std::move(ref1));
        xlnt_assert_equals(ref4, 0); // also moved objects are new objects.
        ++ref4;
        xlnt_assert_equals(ref4, 1);

        xlnt::detail::references ref5;
        xlnt_assert_equals(ref5, 0);
        ref5 = std::move(ref4);
        xlnt_assert_equals(ref5, 0); // idem for move operator
    }
};
static format_impl_test_suite x;
