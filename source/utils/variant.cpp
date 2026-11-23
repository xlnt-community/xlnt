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

#include <xlnt/utils/datetime.hpp>
#include <xlnt/utils/variant.hpp>
#include <xlnt/utils/exceptions.hpp>
#include <unordered_map>

namespace xlnt {

template<>
const std::vector<variant> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_variant)
    {
        return vec_variant_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_variant) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::int8_t &variant::value::get() const
{
    if (internal_type_ == internal_type::i1)
    {
        return i1_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::i1) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::int8_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_i1)
    {
        return vec_i1_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_i1) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::int16_t &variant::value::get() const
{
    if (internal_type_ == internal_type::i2)
    {
        return i2_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::i2) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::int16_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_i2)
    {
        return vec_i2_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_i2) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::int32_t &variant::value::get() const
{
    if (internal_type_ == internal_type::i4)
    {
        return i4_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::i4) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::int32_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_i4)
    {
        return vec_i4_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_i4) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::int64_t &variant::value::get() const
{
    if (internal_type_ == internal_type::i8)
    {
        return i8_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::i8) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::int64_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_i8)
    {
        return vec_i8_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_i8) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::uint8_t &variant::value::get() const
{
    if (internal_type_ == internal_type::ui1)
    {
        return ui1_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::ui1) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::uint8_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_ui1)
    {
        return vec_ui1_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_ui1) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::uint16_t &variant::value::get() const
{
    if (internal_type_ == internal_type::ui2)
    {
        return ui2_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::ui2) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::uint16_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_ui2)
    {
        return vec_ui2_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_ui2) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::uint32_t &variant::value::get() const
{
    if (internal_type_ == internal_type::ui4)
    {
        return ui4_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::ui4) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::uint32_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_ui4)
    {
        return vec_ui4_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_ui4) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::uint64_t &variant::value::get() const
{
    if (internal_type_ == internal_type::ui8)
    {
        return ui8_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::ui8) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::uint64_t> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_ui8)
    {
        return vec_ui8_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_ui8) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const float &variant::value::get() const
{
    if (internal_type_ == internal_type::r4)
    {
        return r4_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::r4) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<float> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_r4)
    {
        return vec_r4_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_r4) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const double &variant::value::get() const
{
    if (internal_type_ == internal_type::r8)
    {
        return r8_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::r8) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<double> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_r8)
    {
        return vec_r8_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_r8) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::string &variant::value::get() const
{
    if (internal_type_ == internal_type::str)
    {
        return str_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::str) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<std::string> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_str)
    {
        return vec_str_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_str) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const xlnt::datetime &variant::value::get() const
{
    if (internal_type_ == internal_type::datetime)
    {
        return datetime_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::datetime) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<xlnt::datetime> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_datetime)
    {
        return vec_datetime_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_datetime) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const bool &variant::value::get() const
{
    if (internal_type_ == internal_type::boolean)
    {
        return boolean_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::boolean) +
        " but got " + get_internal_type_string(internal_type_));
}

template<>
const std::vector<bool> &variant::value::get() const
{
    if (internal_type_ == internal_type::vec_boolean)
    {
        return vec_boolean_;
    }

    throw xlnt::invalid_parameter("expected internal variant type " +
        get_internal_type_string(internal_type::vec_boolean) +
        " but got " + get_internal_type_string(internal_type_));
}

std::string variant::get_type_string(variant::type type)
{
    // IMPORTANT: these strings are also used for serialization,
    // so they MUST be the same ones as in the OOXML specification!
    // For more details, see the allowed types of CT_Variant.
    switch (type)
    {
    case type::vector:
        return "vector";
    //case type::array:
    //    return "array";
    //case type::blob:
    //    return "blob";
    //case type::oblob:
    //    return "oblob";
    case type::empty:
        return "empty";
    case type::null:
        return "null";
    case type::i1:
        return "i1";
    case type::i2:
        return "i2";
    case type::i4:
        return "i4";
    case type::i8:
        return "i8";
    case type::integer:
        return "int";
    case type::ui1:
        return "ui1";
    case type::ui2:
        return "ui2";
    case type::ui4:
        return "ui4";
    case type::ui8:
        return "ui8";
    case type::uint:
        return "uint";
    case type::r4:
        return "r4";
    case type::r8:
        return "r8";
    //case type::decimal:
    //    return "decimal";
    case type::lpstr:
        return "lpstr";
    case type::lpwstr:
        return "lpwstr";
    case type::bstr:
        return "bstr";
    case type::date:
        return "date";
    case type::filetime:
        return "filetime";
    case type::boolean:
        return "bool";
    //case type::cy:
    //    return "cy";
    //case type::error:
    //    return "error";
    //case type::stream:
    //    return "stream";
    //case type::ostream:
    //    return "ostream";
    //case type::storage:
    //    return "storage";
    //case type::ostorage:
    //    return "ostorage";
    //case type::vstream:
    //    return "vstream";
    //case type::clsid:
    //    return "clsid";
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }

    throw xlnt::invalid_parameter("unknown variant type " + std::to_string(static_cast<int>(type)));
}

std::string variant::get_base_type_string(base_type type)
{
    // IMPORTANT: these strings are also used for serialization,
    // so they MUST be the same ones as in the OOXML specification!
    // For more details, see the allowed types of CT_Array and CT_Vector.
    switch (type)
    {
    case base_type::variant:
        return "variant";
    case base_type::i1:
        return get_type_string(type::i1);
    case base_type::i2:
        return get_type_string(type::i2);
    case base_type::i4:
        return get_type_string(type::i4);
    case base_type::i8: // only for vectors
        return get_type_string(type::i8);
    //case base_type::integer: // only for arrays
    //    return get_type_string(type::integer);
    case base_type::ui1:
        return get_type_string(type::ui1);
    case base_type::ui2:
        return get_type_string(type::ui2);
    case base_type::ui4:
        return get_type_string(type::ui4);
    case base_type::ui8: // only for vectors
        return get_type_string(type::ui8);
    //case base_type::uint: // only for arrays
    //    return get_type_string(type::uint);
    case base_type::r4:
        return get_type_string(type::r4);
    case base_type::r8:
        return get_type_string(type::r8);
    //case base_type::decimal: // only for arrays
    //    return get_type_string(type::decimal);
    case base_type::lpstr: // only for vectors
        return get_type_string(type::lpstr);
    case base_type::lpwstr: // only for vectors
        return get_type_string(type::lpwstr);
    case base_type::bstr:
        return get_type_string(type::bstr);
    case base_type::date:
        return get_type_string(type::date);
    case base_type::filetime: // only for vectors
        return get_type_string(type::filetime);
    case base_type::boolean:
        return get_type_string(type::boolean);
    //case base_type::cy:
    //    return get_type_string(type::cy);
    //case base_type::error:
    //    return get_type_string(type::error);
    //case base_type::clsid: // only for vectors
    //    return get_type_string(type::clsid);
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }

    throw xlnt::invalid_parameter("unknown variant base type " + std::to_string(static_cast<int>(type)));
}

variant::type variant::parse_type_string(const std::string &str)
{
    static const std::unordered_map<std::string, variant::type> types {
        {"vector", type::vector},
        //{"array", type::array},
        //{"blob", type::blob},
        //{"oblob", type::oblob},
        {"empty", type::empty},
        {"null", type::null},
        {"i1", type::i1},
        {"i2", type::i2},
        {"i4", type::i4},
        {"i8", type::i8},
        {"int", type::integer},
        {"ui1", type::ui1},
        {"ui2", type::ui2},
        {"ui4", type::ui4},
        {"ui8", type::ui8},
        {"uint", type::uint},
        {"r4", type::r4},
        {"r8", type::r8},
        //{"decimal", type::decimal},
        {"lpstr", type::lpstr},
        {"lpwstr", type::lpwstr},
        {"bstr", type::bstr},
        {"date", type::date},
        {"filetime", type::filetime},
        {"bool", type::boolean},
        //{"cy", type::cy},
        //{"error", type::error},
        //{"stream", type::stream},
        //{"ostream", type::ostream},
        //{"storage", type::storage},
        //{"ostorage", type::ostorage},
        //{"vstream", type::vstream},
        //{"clsid", type::clsid},
    };

    auto it = types.find(str);
    if (it != types.end())
    {
        return it->second;
    }

    throw xlnt::invalid_parameter("unknown variant type \"" + str + "\"");
}

variant::base_type variant::parse_base_type_string(const std::string &str)
{
    static const std::unordered_map<std::string, variant::base_type> base_types {
        {"variant", base_type::variant},
        {"i1", base_type::i1},
        {"i2", base_type::i2},
        {"i4", base_type::i4},
        {"i8", base_type::i8}, // only for vectors
        //{"int", base_type::integer} // only for arrays
        {"ui1", base_type::ui1},
        {"ui2", base_type::ui2},
        {"ui4", base_type::ui4},
        {"ui8", base_type::ui8}, // only for vectors
        //{"uint", base_type::uint}, // only for arrays
        {"r4", base_type::r4},
        {"r8", base_type::r8},
        //{"decimal", base_type::decimal} // only for arrays
        {"lpstr", base_type::lpstr}, // only for vectors
        {"lpwstr", base_type::lpwstr}, // only for vectors
        {"bstr", base_type::bstr},
        {"date", base_type::date},
        {"filetime", base_type::filetime}, // only for vectors
        {"bool", base_type::boolean},
        //{"cy", base_type::cy},
        //{"error", base_type::error},
        //{"clsid", base_type::clsid}, // only for vectors
    };

    auto it = base_types.find(str);
    if (it != base_types.end())
    {
        return it->second;
    }

    throw xlnt::invalid_parameter("unknown base variant type \"" + str + "\"");
}

variant::variant(type type)
    : type_(type)
    , base_type_(base_type::variant) // there is no corresponding base type
{
    if (type != type::null && type != type::empty)
    {
        throw xlnt::invalid_parameter("expected variant type " +
            get_type_string(type::null) + " or " +
            get_type_string(type::empty) +
            ", but got " + get_type_string(type));
    }
}

variant::variant(std::int8_t value)
    : value_(value)
    , type_(type::i1)
    , base_type_(base_type::i1)
{
}

variant::variant(const std::vector<std::int8_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i1)
{
}

variant::variant(std::vector<std::int8_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::i1)
{
}

variant::variant(const std::initializer_list<std::int8_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i1)
{
}

variant::variant(std::int16_t value)
    : value_(value)
    , type_(type::i2)
    , base_type_(base_type::i2)
{
}

variant::variant(const std::vector<std::int16_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i2)
{
}

variant::variant(std::vector<std::int16_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::i2)
{
}

variant::variant(const std::initializer_list<std::int16_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i2)
{
}

variant::variant(std::int32_t value, type type)
    : value_(value)
    , type_(type)
    , base_type_(base_type::i4)
{
    if (type != type::i4 && type != type::integer)
    {
        throw xlnt::invalid_parameter("expected variant type " +
            get_type_string(type::i4) + " or " +
            get_type_string(type::integer) +
            ", but got " + get_type_string(type));
    }
}

variant::variant(const std::vector<std::int32_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i4)
{
}

variant::variant(std::vector<std::int32_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::i4)
{
}

variant::variant(const std::initializer_list<std::int32_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i4)
{
}

variant::variant(std::int64_t value)
    : value_(value)
    , type_(type::i8)
    , base_type_(base_type::i8)
{
}

variant::variant(const std::vector<std::int64_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i8)
{
}

variant::variant(std::vector<std::int64_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::i8)
{
}

variant::variant(const std::initializer_list<std::int64_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::i8)
{
}

variant::variant(std::uint8_t value)
    : value_(value)
    , type_(type::ui1)
    , base_type_(base_type::ui1)
{
}

variant::variant(const std::vector<std::uint8_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui1)
{
}

variant::variant(std::vector<std::uint8_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::ui1)
{
}

variant::variant(const std::initializer_list<std::uint8_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui1)
{
}

variant::variant(std::uint16_t value)
    : value_(value)
    , type_(type::ui2)
    , base_type_(base_type::ui2)
{
}

variant::variant(const std::vector<std::uint16_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui2)
{
}

variant::variant(std::vector<std::uint16_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::ui2)
{
}

variant::variant(const std::initializer_list<std::uint16_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui2)
{
}

variant::variant(std::uint32_t value, type type)
    : value_(value)
    , type_(type)
    , base_type_(base_type::ui4)
{
    if (type != type::ui4 && type != type::uint)
    {
        throw xlnt::invalid_parameter("expected variant type " +
            get_type_string(type::ui4) + " or " +
            get_type_string(type::uint) +
            ", but got " + get_type_string(type));
    }
}

variant::variant(const std::vector<std::uint32_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui4)
{
}

variant::variant(std::vector<std::uint32_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::ui4)
{
}

variant::variant(const std::initializer_list<std::uint32_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui4)
{
}

variant::variant(std::uint64_t value)
    : value_(value)
    , type_(type::ui8)
    , base_type_(base_type::ui8)
{
}

variant::variant(const std::vector<std::uint64_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui8)
{
}

variant::variant(std::vector<std::uint64_t> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::ui8)
{
}

variant::variant(const std::initializer_list<std::uint64_t> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::ui8)
{
}

variant::variant(float value)
    : value_(value)
    , type_(type::r4)
    , base_type_(base_type::r4)
{
}

variant::variant(const std::vector<float> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::r4)
{
}

variant::variant(std::vector<float> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::r4)
{
}

variant::variant(const std::initializer_list<float> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::r4)
{
}

variant::variant(double value)
    : value_(value)
    , type_(type::r8)
    , base_type_(base_type::r8)
{
}

variant::variant(const std::vector<double> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::r8)
{
}

variant::variant(std::vector<double> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::r8)
{
}

variant::variant(const std::initializer_list<double> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::r8)
{
}

void throw_on_invalid_str_type(variant::type type)
{
    if (type != variant::type::lpstr && type != variant::type::lpwstr && type != variant::type::bstr)
    {
        throw xlnt::invalid_parameter("expected variant type " +
            variant::get_type_string(variant::type::lpstr) + " or " +
            variant::get_type_string(variant::type::lpwstr) + " or " +
            variant::get_type_string(variant::type::bstr) +
            ", but got " + variant::get_type_string(type));
    }
}

void throw_on_invalid_str_vec_type(variant::base_type base_type)
{
    if (base_type != variant::base_type::lpstr && base_type != variant::base_type::lpwstr && base_type != variant::base_type::bstr)
    {
        throw xlnt::invalid_parameter("expected variant base type " +
            variant::get_base_type_string(variant::base_type::lpstr) + " or " +
            variant::get_base_type_string(variant::base_type::lpwstr) + " or " +
            variant::get_base_type_string(variant::base_type::bstr) +
            ", but got " + variant::get_base_type_string(base_type));
    }
}

variant::variant(const std::string &value, type type)
    : value_(value)
    , type_(type)
    , base_type_(
        type == type::lpwstr ? base_type::lpwstr :
        (type == type::bstr ? base_type::bstr : base_type::lpstr))
{
    throw_on_invalid_str_type(type);
}

variant::variant(std::string &&value, type type)
    : value_(std::move(value))
    , type_(type)
    , base_type_(
        type == type::lpwstr ? base_type::lpwstr :
        (type == type::bstr ? base_type::bstr : base_type::lpstr))
{
    throw_on_invalid_str_type(type);
}

variant::variant(const char *value, type type)
    : value_(value)
    , type_(type)
    , base_type_(
        type == type::lpwstr ? base_type::lpwstr :
        (type == type::bstr ? base_type::bstr : base_type::lpstr)
    )
{
    throw_on_invalid_str_type(type);
}

variant::variant(const std::vector<std::string> &value, base_type base_type)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_str_vec_type(base_type);
}

variant::variant(std::vector<std::string> &&value, base_type base_type)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_str_vec_type(base_type);
}

variant::variant(const std::vector<const char *> &value, base_type base_type)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_str_vec_type(base_type);
}

variant::variant(const std::initializer_list<std::string> &value, base_type base_type)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_str_vec_type(base_type);
}

variant::variant(const std::initializer_list<const char *> &value, base_type base_type)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_str_vec_type(base_type);
}

void throw_on_invalid_datetime_type(variant::type type)
{
    if (type != variant::type::date && type != variant::type::filetime)
    {
        throw xlnt::invalid_parameter("expected variant type " +
            variant::get_type_string(variant::type::date) + " or " +
            variant::get_type_string(variant::type::filetime) +
            ", but got " + variant::get_type_string(type));
    }
}

void throw_on_invalid_datetime_vec_type(variant::base_type base_type)
{
    if (base_type != variant::base_type::date && base_type != variant::base_type::filetime)
    {
        throw xlnt::invalid_parameter("expected variant base type " +
            variant::get_base_type_string(variant::base_type::date) + " or " +
            variant::get_base_type_string(variant::base_type::filetime) +
            ", but got " + variant::get_base_type_string(base_type));
    }
}

variant::variant(const datetime &value, type type)
    : value_(value)
    , type_(type)
    , base_type_(type == type::filetime ? base_type::filetime : base_type::date)
{
    throw_on_invalid_datetime_type(type);
}

variant::variant(datetime &&value, type type)
    : value_(std::move(value))
    , type_(type)
    , base_type_(type == type::filetime ? base_type::filetime : base_type::date)
{
    throw_on_invalid_datetime_type(type);
}

variant::variant(const std::vector<datetime> &value, base_type base_type)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_datetime_vec_type(base_type);
}

variant::variant(std::vector<datetime> &&value, base_type base_type)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_datetime_vec_type(base_type);
}

variant::variant(const std::initializer_list<datetime> &value, base_type base_type)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type)
{
    throw_on_invalid_datetime_vec_type(base_type);
}

variant::variant(bool value)
    : value_(value)
    , type_(type::boolean)
    , base_type_(base_type::boolean)
{
}

variant::variant(const std::vector<bool> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::boolean)
{
}

variant::variant(std::vector<bool> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::boolean)
{
}

variant::variant(const std::initializer_list<bool> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::boolean)
{
}

variant::variant(const std::vector<variant> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::variant)
{
}

variant::variant(const std::initializer_list<variant> &value)
    : value_(value)
    , type_(type::vector)
    , base_type_(base_type::variant)
{
}

variant::variant(std::vector<variant> &&value)
    : value_(std::move(value))
    , type_(type::vector)
    , base_type_(base_type::variant)
{
}

bool variant::operator==(const variant &rhs) const
{
    return type_ == rhs.type_ && base_type_ == rhs.base_type_ && value_ == rhs.value_;
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
std::int8_t variant::get() const
{
    if (type_ == type::i1)
    {
        return value_.get<std::int8_t>();
    }

    throw xlnt::bad_variant_access(internal_type::i1, *this);
}

template <>
std::vector<std::int8_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::i1)
    {
        return value_.get<std::vector<std::int8_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_i1, *this);
}

template <>
std::int16_t variant::get() const
{
    if (type_ == type::i2)
    {
        return value_.get<std::int16_t>();
    }

    throw xlnt::bad_variant_access(internal_type::i2, *this);
}

template <>
std::vector<std::int16_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::i2)
    {
        return value_.get<std::vector<std::int16_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_i2, *this);
}

template <>
std::int32_t variant::get() const
{
    if (type_ == type::i4 || type_ == type::integer)
    {
        return value_.get<std::int32_t>();
    }

    throw xlnt::bad_variant_access(internal_type::i4, *this);
}

template <>
std::vector<std::int32_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::i4)
    {
        return value_.get<std::vector<std::int32_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_i4, *this);
}

template <>
std::int64_t variant::get() const
{
    if (type_ == type::i8)
    {
        return value_.get<std::int64_t>();
    }

    throw xlnt::bad_variant_access(internal_type::i8, *this);
}

template <>
std::vector<std::int64_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::i8)
    {
        return value_.get<std::vector<std::int64_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_i8, *this);
}

template <>
std::uint8_t variant::get() const
{
    if (type_ == type::ui1)
    {
        return value_.get<std::uint8_t>();
    }

    throw xlnt::bad_variant_access(internal_type::ui1, *this);
}

template <>
std::vector<std::uint8_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::ui1)
    {
        return value_.get<std::vector<std::uint8_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_ui1, *this);
}

template <>
std::uint16_t variant::get() const
{
    if (type_ == type::ui2)
    {
        return value_.get<std::uint16_t>();
    }

    throw xlnt::bad_variant_access(internal_type::ui2, *this);
}

template <>
std::vector<std::uint16_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::ui2)
    {
        return value_.get<std::vector<std::uint16_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_ui2, *this);
}

template <>
std::uint32_t variant::get() const
{
    if (type_ == type::ui4 || type_ == type::uint)
    {
        return value_.get<std::uint32_t>();
    }

    throw xlnt::bad_variant_access(internal_type::ui4, *this);
}

template <>
std::vector<std::uint32_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::ui4)
    {
        return value_.get<std::vector<std::uint32_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_ui4, *this);
}

template <>
std::uint64_t variant::get() const
{
    if (type_ == type::ui8)
    {
        return value_.get<std::uint64_t>();
    }

    throw xlnt::bad_variant_access(internal_type::ui8, *this);
}

template <>
std::vector<std::uint64_t> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::ui8)
    {
        return value_.get<std::vector<std::uint64_t>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_ui8, *this);
}

template <>
float variant::get() const
{
    if (type_ == type::r4)
    {
        return value_.get<float>();
    }

    throw xlnt::bad_variant_access(internal_type::r4, *this);
}

template <>
std::vector<float> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::r4)
    {
        return value_.get<std::vector<float>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_r4, *this);
}

template <>
double variant::get() const
{
    if (type_ == type::r8)
    {
        return value_.get<double>();
    }

    throw xlnt::bad_variant_access(internal_type::r8, *this);
}

template <>
std::vector<double> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::r8)
    {
        return value_.get<std::vector<double>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_r8, *this);
}

template <>
std::string variant::get() const
{
    if (type_ == type::lpstr || type_ == type::lpwstr || type_ == type::bstr)
    {
        return value_.get<std::string>();
    }
    // DEPRECATED, will be removed in XLNT 2.0
    else if (type_ == type::date)
    {
        return value_.get<xlnt::datetime>().to_iso_string();
    }

    throw xlnt::bad_variant_access(internal_type::str, *this);
}

template <>
std::vector<std::string> variant::get() const
{
    if (type_ == type::vector &&
        (base_type_ == base_type::lpstr || base_type_ == base_type::lpwstr
            || base_type_ == base_type::bstr))
    {
        return value_.get<std::vector<std::string>>();
    }
    // DEPRECATED, will be removed in XLNT 2.0
    else if (type_ == type::vector && base_type_ == base_type::date)
    {
        const auto &dates = value_.get<std::vector<xlnt::datetime>>();
        std::vector<std::string> dates_str;
        dates_str.reserve(dates.size());
        for (const xlnt::datetime &date : dates)
        {
            dates_str.emplace_back(date.to_iso_string());
        }

        return dates_str;
    }

    throw xlnt::bad_variant_access(internal_type::vec_str, *this);
}

template <>
datetime variant::get() const
{
    if (type_ == type::date || type_ == type::filetime)
    {
        return value_.get<xlnt::datetime>();
    }

    throw xlnt::bad_variant_access(internal_type::datetime, *this);
}

template <>
std::vector<datetime> variant::get() const
{
    if (type_ == type::vector &&
        (base_type_ == base_type::date || base_type_ == base_type::filetime))
    {
        return value_.get<std::vector<xlnt::datetime>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_datetime, *this);
}

template <>
bool variant::get() const
{
    if (type_ == type::boolean)
    {
        return value_.get<bool>();
    }

    throw xlnt::bad_variant_access(internal_type::boolean, *this);
}

template <>
std::vector<bool> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::boolean)
    {
        return value_.get<std::vector<bool>>();
    }

    throw xlnt::bad_variant_access(internal_type::vec_boolean, *this);
}

template<typename T>
std::vector<variant> vector_convert_to_variant_vec(const std::vector<T> &values)
{
    std::vector<xlnt::variant> values_variant;
    values_variant.reserve(values.size());
    for (const T &value : values)
    {
        values_variant.emplace_back(value);
    }

    return values_variant;
}

template <>
std::vector<variant> variant::get() const
{
    if (type_ == type::vector && base_type_ == base_type::variant)
    {
        return value_.get<std::vector<variant>>();
    }
    // DEPRECATED, will be removed in XLNT 2.0
    else if (type_ == type::vector && base_type_ == base_type::i4)
    {
        return vector_convert_to_variant_vec(value_.get<std::vector<std::int32_t>>());
    }
    // DEPRECATED, will be removed in XLNT 2.0
    else if (type_ == type::vector && base_type_ == base_type::lpstr)
    {
        return vector_convert_to_variant_vec(value_.get<std::vector<std::string>>());
    }
    // DEPRECATED, will be removed in XLNT 2.0
    else if (type_ == type::vector && base_type_ == base_type::date)
    {
        return vector_convert_to_variant_vec(value_.get<std::vector<xlnt::datetime>>());
    }
    // DEPRECATED, will be removed in XLNT 2.0
    else if (type_ == type::vector && base_type_ == base_type::boolean)
    {
        return vector_convert_to_variant_vec(value_.get<std::vector<bool>>());
    }

    throw xlnt::bad_variant_access(internal_type::vec_variant, *this);
}

std::string variant::get_internal_type_string(internal_type type)
{
    switch (type)
    {
    case internal_type::none:
        return "none";
    case internal_type::vec_variant:
        return "vec_variant";
    case internal_type::i1:
        return "i1";
    case internal_type::vec_i1:
        return "vec_i1";
    case internal_type::i2:
        return "i2";
    case internal_type::vec_i2:
        return "vec_i2";
    case internal_type::i4:
        return "i4";
    case internal_type::vec_i4:
        return "vec_i4";
    case internal_type::i8:
        return "i8";
    case internal_type::vec_i8:
        return "vec_i8";
    case internal_type::ui1:
        return "ui1";
    case internal_type::vec_ui1:
        return "vec_ui1";
    case internal_type::ui2:
        return "ui2";
    case internal_type::vec_ui2:
        return "vec_ui2";
    case internal_type::ui4:
        return "ui4";
    case internal_type::vec_ui4:
        return "vec_ui4";
    case internal_type::ui8:
        return "ui8";
    case internal_type::vec_ui8:
        return "vec_ui8";
    case internal_type::r4:
        return "r4";
    case internal_type::vec_r4:
        return "vec_r4";
    case internal_type::r8:
        return "r8";
    case internal_type::vec_r8:
        return "vec_r8";
    case internal_type::str:
        return "str";
    case internal_type::vec_str:
        return "vec_str";
    case internal_type::datetime:
        return "datetime";
    case internal_type::vec_datetime:
        return "vec_datetime";
    case internal_type::boolean:
        return "boolean";
    case internal_type::vec_boolean:
        return "vec_boolean";
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }

    throw xlnt::invalid_parameter("unknown variant internal type " + std::to_string(static_cast<int>(type)));
}

variant::value::value(const std::vector<const char *> &vec_str)
    : internal_type_(internal_type::vec_str)
{
    vec_str_.reserve(vec_str.size());
    for (const char *str : vec_str)
    {
        vec_str_.emplace_back(str);
    }
}

variant::value::value(const std::initializer_list<const char *> &list_str)
    : internal_type_(internal_type::vec_str)
{
    vec_str_.reserve(list_str.size());
    for (const char *str : list_str)
    {
        vec_str_.emplace_back(str);
    }
}

variant::value::value(const value &other)
{
    copy(other);
}

variant::value::value(value &&other) noexcept
{
    move(std::move(other));
}

variant::value &variant::value::operator=(const value &other)
{
    if (this != &other)
    {
        copy(other);
    }
    return *this;
}

variant::value &variant::value::operator=(value &&other) noexcept
{
    if (this != &other)
    {
        move(std::move(other));
    }
    return *this;
}

variant::value::~value()
{
    destroy();
}

std::size_t variant::value::num_elements() const
{
    switch (internal_type_)
    {
    case internal_type::i1:
    case internal_type::i2:
    case internal_type::i4:
    case internal_type::i8:
    case internal_type::ui1:
    case internal_type::ui2:
    case internal_type::ui4:
    case internal_type::ui8:
    case internal_type::r4:
    case internal_type::r8:
    case internal_type::str:
    case internal_type::datetime:
    case internal_type::boolean:
        return 1;
    case internal_type::none:
        return 0;
    case internal_type::vec_variant:
        return vec_variant_.size();
    case internal_type::vec_i1:
        return vec_i1_.size();
    case internal_type::vec_i2:
        return vec_i2_.size();
    case internal_type::vec_i4:
        return vec_i4_.size();
    case internal_type::vec_i8:
        return vec_i8_.size();
    case internal_type::vec_ui1:
        return vec_ui1_.size();
    case internal_type::vec_ui2:
        return vec_ui2_.size();
    case internal_type::vec_ui4:
        return vec_ui4_.size();
    case internal_type::vec_ui8:
        return vec_ui8_.size();
    case internal_type::vec_r4:
        return vec_r4_.size();
    case internal_type::vec_r8:
        return vec_r8_.size();
    case internal_type::vec_str:
        return vec_str_.size();
    case internal_type::vec_datetime:
        return vec_datetime_.size();
    case internal_type::vec_boolean:
        return vec_boolean_.size();
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }

    throw xlnt::invalid_parameter("unknown variant internal type " + std::to_string(static_cast<int>(internal_type_)));
}

void variant::value::copy(const value &other)
{
    destroy();
    internal_type_ = other.internal_type_;

    switch (other.internal_type_)
    {
    case internal_type::vec_variant:
        new (&vec_variant_) std::vector<variant>(other.vec_variant_);
        break;
    case internal_type::i1:
        i1_ = other.i1_;
        break;
    case internal_type::vec_i1:
        new (&vec_i1_) std::vector<std::int8_t>(other.vec_i1_);
        break;
    case internal_type::i2:
        i2_ = other.i2_;
        break;
    case internal_type::vec_i2:
        new (&vec_i2_) std::vector<std::int16_t>(other.vec_i2_);
        break;
    case internal_type::i4:
        i4_ = other.i4_;
        break;
    case internal_type::vec_i4:
        new (&vec_i4_) std::vector<std::int32_t>(other.vec_i4_);
        break;
    case internal_type::i8:
        i8_ = other.i8_;
        break;
    case internal_type::vec_i8:
        new (&vec_i8_) std::vector<std::int64_t>(other.vec_i8_);
        break;
    case internal_type::ui1:
        ui1_ = other.ui1_;
        break;
    case internal_type::vec_ui1:
        new (&vec_ui1_) std::vector<std::uint8_t>(other.vec_ui1_);
        break;
    case internal_type::ui2:
        ui2_ = other.ui2_;
        break;
    case internal_type::vec_ui2:
        new (&vec_ui2_) std::vector<std::uint16_t>(other.vec_ui2_);
        break;
    case internal_type::ui4:
        ui4_ = other.ui4_;
        break;
    case internal_type::vec_ui4:
        new (&vec_ui4_) std::vector<std::uint32_t>(other.vec_ui4_);
        break;
    case internal_type::ui8:
        ui8_ = other.ui8_;
        break;
    case internal_type::vec_ui8:
        new (&vec_ui8_) std::vector<std::uint64_t>(other.vec_ui8_);
        break;
    case internal_type::r4:
        r4_ = other.r4_;
        break;
    case internal_type::vec_r4:
        new (&vec_r4_) std::vector<float>(other.vec_r4_);
        break;
    case internal_type::r8:
        r8_ = other.r8_;
        break;
    case internal_type::vec_r8:
        new (&vec_r8_) std::vector<double>(other.vec_r8_);
        break;
    case internal_type::str:
        new (&str_) std::string(other.str_);
        break;
    case internal_type::vec_str:
        new (&vec_str_) std::vector<std::string>(other.vec_str_);
        break;
    case internal_type::datetime:
        new (&datetime_) xlnt::datetime(other.datetime_);
        break;
    case internal_type::vec_datetime:
        new (&vec_datetime_) std::vector<xlnt::datetime>(other.vec_datetime_);
        break;
    case internal_type::boolean:
        boolean_ = other.boolean_;
        break;
    case internal_type::vec_boolean:
        new (&vec_boolean_) std::vector<bool>(other.vec_boolean_);
        break;
    case internal_type::none:
        break;
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }
}

void variant::value::move(value &&other) noexcept
{
    destroy();
    internal_type_ = other.internal_type_;

    switch (other.internal_type_)
    {
    case internal_type::vec_variant:
        new (&vec_variant_) std::vector<variant>(std::move(other.vec_variant_));
        break;
    case internal_type::i1:
        i1_ = other.i1_;
        break;
    case internal_type::vec_i1:
        new (&vec_i1_) std::vector<std::int8_t>(std::move(other.vec_i1_));
        break;
    case internal_type::i2:
        i2_ = other.i2_;
        break;
    case internal_type::vec_i2:
        new (&vec_i2_) std::vector<std::int16_t>(std::move(other.vec_i2_));
        break;
    case internal_type::i4:
        i4_ = other.i4_;
        break;
    case internal_type::vec_i4:
        new (&vec_i4_) std::vector<std::int32_t>(std::move(other.vec_i4_));
        break;
    case internal_type::i8:
        i8_ = other.i8_;
        break;
    case internal_type::vec_i8:
        new (&vec_i8_) std::vector<std::int64_t>(std::move(other.vec_i8_));
        break;
    case internal_type::ui1:
        ui1_ = other.ui1_;
        break;
    case internal_type::vec_ui1:
        new (&vec_ui1_) std::vector<std::uint8_t>(std::move(other.vec_ui1_));
        break;
    case internal_type::ui2:
        ui2_ = other.ui2_;
        break;
    case internal_type::vec_ui2:
        new (&vec_ui2_) std::vector<std::uint16_t>(std::move(other.vec_ui2_));
        break;
    case internal_type::ui4:
        ui4_ = other.ui4_;
        break;
    case internal_type::vec_ui4:
        new (&vec_ui4_) std::vector<std::uint32_t>(std::move(other.vec_ui4_));
        break;
    case internal_type::ui8:
        ui8_ = other.ui8_;
        break;
    case internal_type::vec_ui8:
        new (&vec_ui8_) std::vector<std::uint64_t>(std::move(other.vec_ui8_));
        break;
    case internal_type::r4:
        r4_ = other.r4_;
        break;
    case internal_type::vec_r4:
        new (&vec_r4_) std::vector<float>(std::move(other.vec_r4_));
        break;
    case internal_type::r8:
        r8_ = other.r8_;
        break;
    case internal_type::vec_r8:
        new (&vec_r8_) std::vector<double>(std::move(other.vec_r8_));
        break;
    case internal_type::str:
        new (&str_) std::string(std::move(other.str_));
        break;
    case internal_type::vec_str:
        new (&vec_str_) std::vector<std::string>(std::move(other.vec_str_));
        break;
    case internal_type::datetime:
        new (&datetime_) xlnt::datetime(std::move(other.datetime_));
        break;
    case internal_type::vec_datetime:
        new (&vec_datetime_) std::vector<xlnt::datetime>(std::move(other.vec_datetime_));
        break;
    case internal_type::boolean:
        boolean_ = other.boolean_;
        break;
    case internal_type::vec_boolean:
        new (&vec_boolean_) std::vector<bool>(std::move(other.vec_boolean_));
        break;
    case internal_type::none:
        break;
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }
}

void variant::value::destroy()
{
    switch (internal_type_)
    {
    case internal_type::vec_variant:
        vec_variant_.std::vector<variant>::vector::~vector();
        break;
    case internal_type::vec_i1:
        vec_i1_.std::vector<std::int8_t>::vector::~vector();
        break;
    case internal_type::vec_i2:
        vec_i2_.std::vector<std::int16_t>::vector::~vector();
        break;
    case internal_type::vec_i4:
        vec_i4_.std::vector<std::int32_t>::vector::~vector();
        break;
    case internal_type::vec_i8:
        vec_i8_.std::vector<std::int64_t>::vector::~vector();
        break;
    case internal_type::vec_ui1:
        vec_ui1_.std::vector<std::uint8_t>::vector::~vector();
        break;
    case internal_type::vec_ui2:
        vec_ui2_.std::vector<std::uint16_t>::vector::~vector();
        break;
    case internal_type::vec_ui4:
        vec_ui4_.std::vector<std::uint32_t>::vector::~vector();
        break;
    case internal_type::vec_ui8:
        vec_ui8_.std::vector<std::uint64_t>::vector::~vector();
        break;
    case internal_type::vec_r4:
        vec_r4_.std::vector<float>::vector::~vector();
        break;
    case internal_type::vec_r8:
        vec_r8_.std::vector<double>::vector::~vector();
        break;
    case internal_type::str:
        str_.std::string::~string();
        break;
    case internal_type::vec_str:
        vec_str_.std::vector<std::string>::vector::~vector();
        break;
    case internal_type::datetime:
        datetime_.xlnt::datetime::~datetime();
        break;
    case internal_type::vec_datetime:
        vec_datetime_.std::vector<xlnt::datetime>::vector::~vector();
        break;
    case internal_type::vec_boolean:
        vec_boolean_.std::vector<bool>::vector::~vector();
        break;
    case internal_type::none:
    case internal_type::i1:
    case internal_type::i2:
    case internal_type::i4:
    case internal_type::i8:
    case internal_type::ui1:
    case internal_type::ui2:
    case internal_type::ui4:
    case internal_type::ui8:
    case internal_type::r4:
    case internal_type::r8:
    case internal_type::boolean:
        break;
    // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
    }

    // The following is strictly necessary only for the types actually destroyed above.
    // However, allowing access in this case would be bad practice,
    // so let's always forbid it.
    internal_type_ = internal_type::none;
}

bool variant::value::operator==(const value &other) const
{
    bool equal = internal_type_ == other.internal_type_;

    if (equal)
    {
        switch (internal_type_)
        {
        case internal_type::vec_variant:
            equal = vec_variant_ == other.vec_variant_;
            break;
        case internal_type::i1:
            equal = i1_ == other.i1_;
            break;
        case internal_type::vec_i1:
            equal = vec_i1_ == other.vec_i1_;
            break;
        case internal_type::i2:
            equal = i2_ == other.i2_;
            break;
        case internal_type::vec_i2:
            equal = vec_i2_ == other.vec_i2_;
            break;
        case internal_type::i4:
            equal = i4_ == other.i4_;
            break;
        case internal_type::vec_i4:
            equal = vec_i4_ == other.vec_i4_;
            break;
        case internal_type::i8:
            equal = i8_ == other.i8_;
            break;
        case internal_type::vec_i8:
            equal = vec_i8_ == other.vec_i8_;
            break;
        case internal_type::ui1:
            equal = ui1_ == other.ui1_;
            break;
        case internal_type::vec_ui1:
            equal = vec_ui1_ == other.vec_ui1_;
            break;
        case internal_type::ui2:
            equal = ui2_ == other.ui2_;
            break;
        case internal_type::vec_ui2:
            equal = vec_ui2_ == other.vec_ui2_;
            break;
        case internal_type::ui4:
            equal = ui4_ == other.ui4_;
            break;
        case internal_type::vec_ui4:
            equal = vec_ui4_ == other.vec_ui4_;
            break;
        case internal_type::ui8:
            equal = ui8_ == other.ui8_;
            break;
        case internal_type::vec_ui8:
            equal = vec_ui8_ == other.vec_ui8_;
            break;
        case internal_type::r4:
            equal = r4_ == other.r4_;
            break;
        case internal_type::vec_r4:
            equal = vec_r4_ == other.vec_r4_;
            break;
        case internal_type::r8:
            equal = r8_ == other.r8_;
            break;
        case internal_type::vec_r8:
            equal = vec_r8_ == other.vec_r8_;
            break;
        case internal_type::str:
            equal = str_ == other.str_;
            break;
        case internal_type::vec_str:
            equal = vec_str_ == other.vec_str_;
            break;
        case internal_type::datetime:
            equal = datetime_ == other.datetime_;
            break;
        case internal_type::vec_datetime:
            equal = vec_datetime_ == other.vec_datetime_;
            break;
        case internal_type::boolean:
            equal = boolean_ == other.boolean_;
            break;
        case internal_type::vec_boolean:
            equal = vec_boolean_ == other.vec_boolean_;
            break;
        case internal_type::none:
            equal = true;
            break;
        // no "default" so that compiler warnings are (hopefully) displayed when a "case" is forgotten
        }
    }

    return equal;
}

bool variant::value::operator!=(const value &other) const
{
    return !(*this == other);
}

} // namespace xlnt
