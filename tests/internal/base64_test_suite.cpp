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

#include <iostream>
#include <vector>
#include <string>

#include <helpers/test_suite.hpp>
#include <xlnt/xlnt.hpp>
#include <xlnt/utils/exceptions.hpp>

#include <detail/cryptography/base64.hpp>

using namespace xlnt::detail;

class base64_test_suite : public test_suite
{
public:
    base64_test_suite()
    {
        register_test(test_decode_empty);
        register_test(test_decode_malformed_length);
        register_test(test_decode_malformed_padding);
        register_test(test_decode_invalid_chars);
        register_test(test_issue137_payload);
    }

    void test_decode_empty()
    {
        std::string input = "";
        auto output = decode_base64(input);
        xlnt_assert(output.empty());
    }

    void test_decode_malformed_length()
    {
        std::vector<std::string> inputs = {
            "SGVsbG8",
            "A",
            "AB",
            "ABC"
        };

        for (const auto& in : inputs)
        {
            try 
            {
                auto output = decode_base64(in);
                (void)output;
            }
            catch (...) 
            {
            }
        }
        xlnt_assert(true);
    }

    void test_decode_malformed_padding()
    {
        std::vector<std::string> inputs = {
            "====",
            "A===",
            "AB=="
        };

        for (const auto& in : inputs)
        {
            try 
            {
                auto output = decode_base64(in);
                (void)output;
            }
            catch (...) 
            {
            }
        }
        xlnt_assert(true);
    }

    void test_decode_invalid_chars()
    {
        std::string input = "$#@!";
        try 
        {
            auto output = decode_base64(input);
            (void)output;
        }
        catch (...) {}
        xlnt_assert(true);
    }

    void test_issue137_payload()
    {
        std::string input = "Ws7Lk2ZRUg52XqgmyE8Nkzx7p9wRpXy8zkpiIZw/calcChain3Ji0yae3jfy2N1q9u6fmuj3vUDE20DSF6Lt1iNUwhQ8Hfg==";
        
        try 
        {
            auto output = decode_base64(input);
            (void)output;
        }
        catch (...) 
        {
            
        }
        xlnt_assert(true);
    }
};

static base64_test_suite x;
