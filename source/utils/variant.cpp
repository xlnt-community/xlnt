// Copyright (c) 2017-2022 Thomas Fussell
// Copyright (c) 2024-2025 xlnt-community
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

#include <xlnt/utils/datetime.hpp>
#include <xlnt/utils/variant.hpp>
#include <xlnt/utils/exceptions.hpp>

namespace xlnt {

variant::variant()
    : type_(type::null)
{
}

variant::variant(const std::string &value)
    : type_(type::lpstr),
      lpstr_value_(value)
{
}

variant::variant(const char *value)
    : variant(std::string(value))
{
}

variant::variant(int32_t value)
    : type_(type::i4),
      i4_value_(value)
{
}

variant::variant(bool value)
    : type_(type::boolean),
      i4_value_(value ? 1 : 0)
{
}

variant::variant(const datetime &value)
    : type_(type::date),
      lpstr_value_(value.to_iso_string())
{
}

template<typename T>
void variant::construct_vector_internal(const T &vec)
{
    vector_value_.reserve(vec.size());
    for (const auto &v : vec)
    {
        vector_value_.emplace_back(v);
    }
}

variant::variant(const std::initializer_list<std::int32_t> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::vector<std::int32_t> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::initializer_list<const char *> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::vector<const char *> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::initializer_list<std::string> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::vector<std::string> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::initializer_list<bool> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::vector<bool> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::initializer_list<datetime> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::vector<datetime> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::initializer_list<variant> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

variant::variant(const std::vector<variant> &value)
    : type_(type::vector)
{
    construct_vector_internal(value);
}

bool variant::operator==(const variant &rhs) const
{
    if (type_ != rhs.type_)
    {
        return false;
    }
    switch (type_)
    {
    case type::vector:
        return vector_value_ == rhs.vector_value_;
    case type::i4:
    case type::boolean:
        return i4_value_ == rhs.i4_value_;
    case type::date:
    case type::lpstr:
        return lpstr_value_ == rhs.lpstr_value_;
    case type::null:
        return true;
    }
    return false;
}

bool variant::operator!=(const variant &rhs) const
{
    return !(*this == rhs);
}

bool variant::is(type t) const
{
    return type_ == t;
}

template <>
std::string variant::get() const
{
    if (type_ == type::lpstr)
    {
        return lpstr_value_;
    }

    throw xlnt::bad_variant_access(static_cast<int>(type::lpstr), static_cast<int>(type_));
}

template <>
bool variant::get() const
{
    if (type_ == type::boolean)
    {
        return i4_value_ != 0;
    }

    throw xlnt::bad_variant_access(static_cast<int>(type::boolean), static_cast<int>(type_));
}

template <>
std::int32_t variant::get() const
{
    if (type_ == type::i4)
    {
        return i4_value_;
    }

    throw xlnt::bad_variant_access(static_cast<int>(type::i4), static_cast<int>(type_));
}

template <>
datetime variant::get() const
{
    if (type_ == type::date)
    {
        return datetime::from_iso_string(lpstr_value_);
    }

    throw xlnt::bad_variant_access(static_cast<int>(type::date), static_cast<int>(type_));
}

template <>
std::vector<variant> variant::get() const
{
    if (type_ == type::vector)
    {
        return vector_value_;
    }

    throw xlnt::bad_variant_access(static_cast<int>(type::vector), static_cast<int>(type_));
}

template<typename T>
std::vector<T> variant::get_vector_internal() const
{
    if (type_ != type::vector)
    {
        throw xlnt::bad_variant_access(static_cast<int>(type::vector), static_cast<int>(type_));
    }

    // According to the OOXML specification, "Vector contents shall be of uniform type"
    std::vector<T> vec;
    vec.reserve(vector_value_.size());
    for (const variant &var : vector_value_)
    {
        vec.emplace_back(var.get<T>());
    }
    return vec;
}

template <>
std::vector<bool> variant::get() const
{
    return get_vector_internal<bool>();
}

template <>
std::vector<std::int32_t> variant::get() const
{
    return get_vector_internal<std::int32_t>();
}

template <>
std::vector<std::string> variant::get() const
{
    return get_vector_internal<std::string>();
}

template <>
std::vector<datetime> variant::get() const
{
    return get_vector_internal<datetime>();
}

variant::type variant::value_type() const
{
    return type_;
}

} // namespace xlnt
