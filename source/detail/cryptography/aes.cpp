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

#include <detail/cryptography/aes.hpp>
#include <xlnt/utils/exceptions.hpp>
#include <openssl/evp.h>
#include <limits>
#include <memory>

namespace {
std::vector<std::uint8_t> transform(const std::vector<std::uint8_t>& input,
    const std::vector<std::uint8_t>& key, const std::vector<std::uint8_t>* iv,
    std::size_t offset, bool encrypt)
{
    if (input.empty()) return {};
    if (offset > input.size() || (input.size() - offset) % 16 != 0 ||
        input.size() - offset > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw xlnt::invalid_parameter("Invalid AES input length or offset");
    if (iv && iv->size() < 16)
        throw xlnt::invalid_parameter("Invalid AES IV length");

    const EVP_CIPHER* cipher = nullptr;
    switch (key.size()) {
    case 16: cipher = iv ? EVP_aes_128_cbc() : EVP_aes_128_ecb(); break;
    case 24: cipher = iv ? EVP_aes_192_cbc() : EVP_aes_192_ecb(); break;
    case 32: cipher = iv ? EVP_aes_256_cbc() : EVP_aes_256_ecb(); break;
    default: throw xlnt::invalid_parameter("Invalid AES key length");
    }
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>
        context(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    if (!context || EVP_CipherInit_ex(context.get(), cipher, nullptr, key.data(),
            iv ? iv->data() : nullptr, encrypt ? 1 : 0) != 1 ||
        EVP_CIPHER_CTX_set_padding(context.get(), 0) != 1)
        throw xlnt::exception("Unable to initialize OpenSSL AES cipher");

    std::vector<std::uint8_t> output(input.size() - offset + 16);
    int length = 0, final_length = 0;
    if (EVP_CipherUpdate(context.get(), output.data(), &length, input.data() + offset,
            static_cast<int>(input.size() - offset)) != 1 ||
        EVP_CipherFinal_ex(context.get(), output.data() + length, &final_length) != 1)
        throw xlnt::exception("OpenSSL AES operation failed");
    output.resize(length + final_length);
    return output;
}
}

namespace xlnt { namespace detail {
std::vector<std::uint8_t> aes_ecb_encrypt(const std::vector<std::uint8_t>& input,
    const std::vector<std::uint8_t>& key, std::size_t offset)
{ return transform(input, key, nullptr, offset, true); }
std::vector<std::uint8_t> aes_ecb_decrypt(const std::vector<std::uint8_t>& input,
    const std::vector<std::uint8_t>& key, std::size_t offset)
{ return transform(input, key, nullptr, offset, false); }
std::vector<std::uint8_t> aes_cbc_encrypt(const std::vector<std::uint8_t>& input,
    const std::vector<std::uint8_t>& key, const std::vector<std::uint8_t>& iv, std::size_t offset)
{ return transform(input, key, &iv, offset, true); }
std::vector<std::uint8_t> aes_cbc_decrypt(const std::vector<std::uint8_t>& input,
    const std::vector<std::uint8_t>& key, const std::vector<std::uint8_t>& iv, std::size_t offset)
{ return transform(input, key, &iv, offset, false); }
}}
