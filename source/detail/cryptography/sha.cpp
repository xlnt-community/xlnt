// Copyright (c) 2017-2022 Thomas Fussell
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

#include <detail/cryptography/sha.hpp>
#include <xlnt/utils/exceptions.hpp>
#include <openssl/evp.h>

namespace {
void digest(const EVP_MD* algorithm, const std::vector<std::uint8_t>& input,
    std::vector<std::uint8_t>& output)
{
    std::vector<std::uint8_t> result(EVP_MAX_MD_SIZE);
    unsigned int length = 0;
    if (EVP_Digest(input.data(), input.size(), result.data(), &length, algorithm, nullptr) != 1)
        throw xlnt::exception("OpenSSL digest operation failed");
    result.resize(length);
    output.swap(result);
}
}
namespace xlnt { namespace detail {
void sha1(const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output)
{ digest(EVP_sha1(), input, output); }
void sha512(const std::vector<std::uint8_t>& input, std::vector<std::uint8_t>& output)
{ digest(EVP_sha512(), input, output); }
}}
