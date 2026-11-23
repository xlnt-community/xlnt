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

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <xlnt/xlnt_config.hpp>
#include <xlnt/utils/datetime.hpp>

namespace xlnt {

struct datetime;
class bad_variant_access;

/// <summary>
/// Represents an object that can have variable type.
/// </summary>
class XLNT_API variant
{
public:
    friend class xlnt::bad_variant_access;

    /// <summary>
    /// The possible types a variant can hold.
    /// </summary>
    enum class type : unsigned char
    {
        vector = 0,
        //array = 1,
        //blob = 2, // XSD type xsd:base64Binary
        //oblob = 3, // XSD type xsd:base64Binary
        empty = 4,
        null = 5,
        i1 = 6, // XSD type xsd:byte (8-bit signed integer)
        i2 = 7, // XSD type xsd:short (16-bit signed integer)
        i4 = 8, // XSD type xsd:int (32-bit signed integer)
        i8 = 9, // XSD type xsd:long (64-bit signed integer)
        integer = 10, // XSD type xsd:int (32-bit signed integer)
        ui1 = 11, // XSD type xsd:unsignedByte (8-bit unsigned integer)
        ui2 = 12, // XSD type xsd:unsignedShort (16-bit unsigned integer)
        ui4 = 13, // XSD type xsd:unsignedInt (32-bit unsigned integer)
        ui8 = 14, // XSD type xsd:unsignedLong (64-bit unsigned integer)
        uint = 15, // XSD type xsd:unsignedInt (32-bit unsigned integer)
        r4 = 16, // XSD type xsd:float (32-bit floating point number)
        r8 = 17, // XSD type xsd:double (64-bit floating point number)
        //decimal = 18, XSD type xsd:decimal
        lpstr = 19, // XSD type xsd:string. OOXML says: "This element specifies a string variant type."
        lpwstr = 20, // XSD type xsd:string. OOXML says: "This element specifies a string variant type."
        bstr = 21, // XSD type xsd:string. OOXML says: "This element defines a binary basic string variant type, which can store any valid Unicode character."
        date = 22, // XSD type xsd:dateTime
        filetime = 23, // XSD type xsd:dateTime
        boolean = 24, // XSD type xsd:boolean (any of: 'true', 'false', '1', '0')
        //cy = 25,
        //error = 26,
        //stream = 27, // XSD type xsd:base64Binary
        //ostream = 28, // XSD type xsd:base64Binary
        //storage = 29, // XSD type xsd:base64Binary
        //ostorage = 30, // XSD type xsd:base64Binary
        //vstream = 31,
        //clsid = 32
    };

    /// <summary>
    /// The possible base types for vectors and arrays.
    /// Note: vectors and arrays support slightly different types!
    /// </summary>
    enum class base_type : unsigned char
    {
        variant = 0,
        i1 = 1,
        i2 = 2,
        i4 = 3,
        i8 = 4,         // only for vectors
        //integer = 5,  // only for arrays
        ui1 = 6,
        ui2 = 7,
        ui4 = 8,
        ui8 = 9,        // only for vectors
        //uint = 10,    // only for arrays
        r4 = 11,
        r8 = 12,
        //decimal = 13, // only for arrays
        lpstr = 14,     // only for vectors
        lpwstr = 15,    // only for vectors
        bstr = 16,
        date = 17,
        filetime = 18,  // only for vectors
        boolean = 19,
        //cy = 20,
        //error = 21,
        //clsid = 22    // only for vectors
    };

    /// <summary>
    /// Returns a displayable string for the specified type.
    /// </summary>
    static std::string get_type_string(type type);
    static std::string get_base_type_string(base_type type);

    /// <summary>
    /// Parse the string and return the specified type.
    /// </summary>
    static type parse_type_string(const std::string &str);
    static base_type parse_base_type_string(const std::string &str);

    /// <summary>
    /// Default constructor. Creates a null-type or empty-type variant.
    /// The type must be either type::null or type::empty - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    explicit variant(type type = type::null);

    /// <summary>
    /// Creates a i1-type variant with the given value.
    /// </summary>
    variant(std::int8_t value);

    /// <summary>
    /// Creates a vector_i1-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::int8_t> &value);
    variant(std::vector<std::int8_t> &&value);
    variant(const std::initializer_list<std::int8_t> &value);

    /// <summary>
    /// Creates a i2-type variant with the given value.
    /// </summary>
    variant(std::int16_t value);

    /// <summary>
    /// Creates a vector_i2-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::int16_t> &value);
    variant(std::vector<std::int16_t> &&value);
    variant(const std::initializer_list<std::int16_t> &value);

    /// <summary>
    /// Creates a i4-type or int-type variant with the given value.
    /// The type must be either type::i4 or type::integer - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    variant(std::int32_t value, type type = type::i4);

    /// <summary>
    /// Creates a vector_i4-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::int32_t> &value);
    variant(std::vector<std::int32_t> &&value);
    variant(const std::initializer_list<std::int32_t> &value);

    /// <summary>
    /// Creates a i8-type variant with the given value.
    /// </summary>
    variant(std::int64_t value);

    /// <summary>
    /// Creates a vector_i8-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::int64_t> &value);
    variant(std::vector<std::int64_t> &&value);
    variant(const std::initializer_list<std::int64_t> &value);

    /// <summary>
    /// Creates a ui1-type variant with the given value.
    /// </summary>
    variant(std::uint8_t value);

    /// <summary>
    /// Creates a vector_ui1-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::uint8_t> &value);
    variant(std::vector<std::uint8_t> &&value);
    variant(const std::initializer_list<std::uint8_t> &value);

    /// <summary>
    /// Creates a ui2-type variant with the given value.
    /// </summary>
    variant(std::uint16_t value);

    /// <summary>
    /// Creates a vector_ui2-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::uint16_t> &value);
    variant(std::vector<std::uint16_t> &&value);
    variant(const std::initializer_list<std::uint16_t> &value);

    /// <summary>
    /// Creates a ui4-type or uint-type variant with the given value.
    /// The type must be either type::ui4 or type::uint - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    variant(std::uint32_t value, type type = type::ui4);

    /// <summary>
    /// Creates a vector_ui4-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::uint32_t> &value);
    variant(std::vector<std::uint32_t> &&value);
    variant(const std::initializer_list<std::uint32_t> &value);

    /// <summary>
    /// Creates a ui8-type variant with the given value.
    /// </summary>
    variant(std::uint64_t value);

    /// <summary>
    /// Creates a vector_ui8-type variant with the given value.
    /// </summary>
    variant(const std::vector<std::uint64_t> &value);
    variant(std::vector<std::uint64_t> &&value);
    variant(const std::initializer_list<std::uint64_t> &value);

    /// <summary>
    /// Creates a r4-type variant with the given value.
    /// </summary>
    variant(float value);

    /// <summary>
    /// Creates a vector_r4-type variant with the given value.
    /// </summary>
    variant(const std::vector<float> &value);
    variant(std::vector<float> &&value);
    variant(const std::initializer_list<float> &value);

    /// <summary>
    /// Creates a r8-type variant with the given value.
    /// </summary>
    variant(double value);

    /// <summary>
    /// Creates a vector_r8-type variant with the given value.
    /// </summary>
    variant(const std::vector<double> &value);
    variant(std::vector<double> &&value);
    variant(const std::initializer_list<double> &value);

    /// <summary>
    /// Creates a string-type variant with the given value.
    /// The type must be either type::lpstr or type::lpwstr or type::bstr - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    variant(const std::string &value, type type = type::lpstr);
    variant(std::string &&value, type type = type::lpstr);
    variant(const char *value, type type = type::lpstr);

    /// <summary>
    /// Creates a vector_string-type variant with the given value.
    /// The base type must be either base_type::lpstr or base_type::lpwstr or base_type::bstr - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    variant(const std::vector<std::string> &value, base_type base_type = base_type::lpstr);
    variant(std::vector<std::string> &&value, base_type base_type = base_type::lpstr);
    variant(const std::vector<const char *> &value, base_type base_type = base_type::lpstr);
    variant(const std::initializer_list<std::string> &value, base_type base_type = base_type::lpstr);
    variant(const std::initializer_list<const char *> &value, base_type base_type = base_type::lpstr);

    /// <summary>
    /// Creates a datetime-type variant with the given value.
    /// The type must be either type::date or type::filetime - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    variant(const datetime &value, type type = type::date);
    variant(datetime &&value, type type = type::date);

    /// <summary>
    /// Creates a vector_datetime-type variant with the given value.
    /// The base type must be either base_type::date or base_type::filetime - if not,
    /// an xlnt::invalid_parameter exception will be thrown.
    /// </summary>
    variant(const std::vector<datetime> &value, base_type base_type = base_type::date);
    variant(std::vector<datetime> &&value, base_type base_type = base_type::date);
    variant(const std::initializer_list<datetime> &value, base_type base_type = base_type::date);

    /// <summary>
    /// Creates a bool-type variant with the given value.
    /// </summary>
    variant(bool value);

    /// <summary>
    /// Creates a vector_bool-type variant with the given value.
    /// </summary>
    variant(const std::vector<bool> &value);
    variant(std::vector<bool> &&value);
    variant(const std::initializer_list<bool> &value);

    /// <summary>
    /// Creates a vector_variant-type variant with the given value.
    /// </summary>
    variant(const std::vector<variant> &value);
    variant(std::vector<variant> &&value);
    variant(const std::initializer_list<variant> &value);

    /// <summary>
    /// Returns true if this variant is of type t.
    /// </summary>
    bool is(type t) const;

    /// <summary>
    /// Returns the value of this variant as type T.
    /// Assumes that the variant is of type T (please call is() or value_type() to check). For details
    /// on each type, please see the documentation for each get<T> function below.
    /// An xlnt::bad_variant_access exception will be thrown if the types are not convertible.
    /// </summary>
    template <typename T>
    T get() const;

    /// <summary>
    /// Returns the type of this variant.
    /// </summary>
    type value_type() const { return type_; }

    /// <summary>
    /// Returns the type of this variant. Only relevant
    /// for vector and array types.
    /// </summary>
    base_type value_base_type() const { return base_type_; }

    /// <summary>
    /// Returns the number of elements encapsulated by this variant.
    /// </summary>
    std::size_t num_elements() const { return value_.num_elements(); }

    bool operator==(const variant &rhs) const;

    bool operator!=(const variant &rhs) const;

private:

    enum class internal_type : unsigned char
    {
        none, // none of the union members are active
        vec_variant, // std::vector<xlnt::variant>
        i1, // std::int8_t
        vec_i1, // std::vector<std::int8_t>
        i2, // std::int16_t
        vec_i2, // std::vector<std::int16_t>
        i4, // std::int32_t
        vec_i4, // std::vector<std::int32_t>
        i8, // std::int64_t
        vec_i8, // std::vector<std::int64_t>
        ui1, // std::uint8_t
        vec_ui1, // std::vector<std::uint8_t>
        ui2, // std::uint16_t
        vec_ui2, // std::vector<std::uint16_t>
        ui4, // std::uint32_t
        vec_ui4, // std::vector<std::uint32_t>
        ui8, // std::uint64_t
        vec_ui8, // std::vector<std::uint64_t>
        r4, // float
        vec_r4, // std::vector<float>
        r8, // double
        vec_r8, // std::vector<double>
        str, // std::string
        vec_str, // std::vector<std::string>
        datetime, // xlnt::datetime
        vec_datetime, // std::vector<xlnt::datetime>
        boolean, // bool
        vec_boolean, // std::vector<bool>
    };

    static std::string get_internal_type_string(internal_type type);

    struct value
    {
    public:
        value() {}

        explicit value(const std::vector<variant> &vec_variant) : vec_variant_(vec_variant), internal_type_(internal_type::vec_variant) {}
        explicit value(std::vector<variant> &&vec_variant) : vec_variant_(std::move(vec_variant)), internal_type_(internal_type::vec_variant) {}
        explicit value(const std::initializer_list<variant> &list_variant) : vec_variant_(list_variant), internal_type_(internal_type::vec_variant) {}

        explicit value(std::int8_t i1) : i1_(i1), internal_type_(internal_type::i1) {}
        explicit value(const std::vector<std::int8_t> &vec_i1) : vec_i1_(vec_i1), internal_type_(internal_type::vec_i1) {}
        explicit value(std::vector<std::int8_t> &&vec_i1) : vec_i1_(std::move(vec_i1)), internal_type_(internal_type::vec_i1) {}
        explicit value(const std::initializer_list<std::int8_t> &list_i1) : vec_i1_(list_i1), internal_type_(internal_type::vec_i1) {}

        explicit value(std::int16_t i2) : i2_(i2), internal_type_(internal_type::i2) {}
        explicit value(const std::vector<std::int16_t> &vec_i2) : vec_i2_(vec_i2), internal_type_(internal_type::vec_i2) {}
        explicit value(std::vector<std::int16_t> &&vec_i2) : vec_i2_(std::move(vec_i2)), internal_type_(internal_type::vec_i2) {}
        explicit value(const std::initializer_list<std::int16_t> &list_i2) : vec_i2_(list_i2), internal_type_(internal_type::vec_i2) {}

        explicit value(std::int32_t i4) : i4_(i4), internal_type_(internal_type::i4) {}
        explicit value(const std::vector<std::int32_t> &vec_i4) : vec_i4_(vec_i4), internal_type_(internal_type::vec_i4) {}
        explicit value(std::vector<std::int32_t> &&vec_i4) : vec_i4_(std::move(vec_i4)), internal_type_(internal_type::vec_i4) {}
        explicit value(const std::initializer_list<std::int32_t> &list_i4) : vec_i4_(list_i4), internal_type_(internal_type::vec_i4) {}

        explicit value(std::int64_t i8) : i8_(i8), internal_type_(internal_type::i8) {}
        explicit value(const std::vector<std::int64_t> &vec_i8) : vec_i8_(vec_i8), internal_type_(internal_type::vec_i8) {}
        explicit value(std::vector<std::int64_t> &&vec_i8) : vec_i8_(std::move(vec_i8)), internal_type_(internal_type::vec_i8) {}
        explicit value(const std::initializer_list<std::int64_t> &list_i8) : vec_i8_(list_i8), internal_type_(internal_type::vec_i8) {}

        explicit value(std::uint8_t ui1) : ui1_(ui1), internal_type_(internal_type::ui1) {}
        explicit value(const std::vector<std::uint8_t> &vec_ui1) : vec_ui1_(vec_ui1), internal_type_(internal_type::vec_ui1) {}
        explicit value(std::vector<std::uint8_t> &&vec_ui1) : vec_ui1_(std::move(vec_ui1)), internal_type_(internal_type::vec_ui1) {}
        explicit value(const std::initializer_list<std::uint8_t> &list_ui1) : vec_ui1_(list_ui1), internal_type_(internal_type::vec_ui1) {}

        explicit value(std::uint16_t ui2) : ui2_(ui2), internal_type_(internal_type::ui2) {}
        explicit value(const std::vector<std::uint16_t> &vec_ui2) : vec_ui2_(vec_ui2), internal_type_(internal_type::vec_ui2) {}
        explicit value(std::vector<std::uint16_t> &&vec_ui2) : vec_ui2_(std::move(vec_ui2)), internal_type_(internal_type::vec_ui2) {}
        explicit value(const std::initializer_list<std::uint16_t> &list_ui2) : vec_ui2_(list_ui2), internal_type_(internal_type::vec_ui2) {}

        explicit value(std::uint32_t ui4) : ui4_(ui4), internal_type_(internal_type::ui4) {}
        explicit value(const std::vector<std::uint32_t> &vec_ui4) : vec_ui4_(vec_ui4), internal_type_(internal_type::vec_ui4) {}
        explicit value(std::vector<std::uint32_t> &&vec_ui4) : vec_ui4_(std::move(vec_ui4)), internal_type_(internal_type::vec_ui4) {}
        explicit value(const std::initializer_list<std::uint32_t> &list_ui4) : vec_ui4_(list_ui4), internal_type_(internal_type::vec_ui4) {}

        explicit value(std::uint64_t ui8) : ui8_(ui8), internal_type_(internal_type::ui8) {}
        explicit value(const std::vector<std::uint64_t> &vec_ui8) : vec_ui8_(vec_ui8), internal_type_(internal_type::vec_ui8) {}
        explicit value(std::vector<std::uint64_t> &&vec_ui8) : vec_ui8_(std::move(vec_ui8)), internal_type_(internal_type::vec_ui8) {}
        explicit value(const std::initializer_list<std::uint64_t> &list_ui8) : vec_ui8_(list_ui8), internal_type_(internal_type::vec_ui8) {}

        explicit value(float r4) : r4_(r4), internal_type_(internal_type::r4) {}
        explicit value(const std::vector<float> &vec_r4) : vec_r4_(vec_r4), internal_type_(internal_type::vec_r4) {}
        explicit value(std::vector<float> &&vec_r4) : vec_r4_(std::move(vec_r4)), internal_type_(internal_type::vec_r4) {}
        explicit value(const std::initializer_list<float> &list_r4) : vec_r4_(list_r4), internal_type_(internal_type::vec_r4) {}

        explicit value(double r8) : r8_(r8), internal_type_(internal_type::r8) {}
        explicit value(const std::vector<double> &vec_r8) : vec_r8_(vec_r8), internal_type_(internal_type::vec_r8) {}
        explicit value(std::vector<double> &&vec_r8) : vec_r8_(std::move(vec_r8)), internal_type_(internal_type::vec_r8) {}
        explicit value(const std::initializer_list<double> &list_r8) : vec_r8_(list_r8), internal_type_(internal_type::vec_r8) {}

        explicit value(const std::string &str) : str_(str), internal_type_(internal_type::str) {}
        explicit value(std::string &&str) : str_(std::move(str)), internal_type_(internal_type::str) {}
        explicit value(const char *str) : str_(str), internal_type_(internal_type::str) {}
        explicit value(const std::vector<std::string> &vec_str) : vec_str_(vec_str), internal_type_(internal_type::vec_str) {}
        explicit value(std::vector<std::string> &&vec_str) : vec_str_(std::move(vec_str)), internal_type_(internal_type::vec_str) {}
        explicit value(const std::vector<const char *> &vec_str);
        explicit value(const std::initializer_list<std::string> &list_str) : vec_str_(list_str), internal_type_(internal_type::vec_str) {}
        explicit value(const std::initializer_list<const char *> &list_str);

        explicit value(const xlnt::datetime &datetime) : datetime_(datetime), internal_type_(internal_type::datetime) {}
        explicit value(xlnt::datetime &&datetime) : datetime_(std::move(datetime)), internal_type_(internal_type::datetime) {}
        explicit value(const std::vector<xlnt::datetime> &vec_datetime) : vec_datetime_(vec_datetime), internal_type_(internal_type::vec_datetime) {}
        explicit value(std::vector<xlnt::datetime> &&vec_datetime) : vec_datetime_(std::move(vec_datetime)), internal_type_(internal_type::vec_datetime) {}
        explicit value(const std::initializer_list<xlnt::datetime> &list_datetime) : vec_datetime_(list_datetime), internal_type_(internal_type::vec_datetime) {}

        explicit value(bool boolean) : boolean_(boolean), internal_type_(internal_type::boolean) {}
        explicit value(const std::vector<bool> &vec_boolean) : vec_boolean_(vec_boolean), internal_type_(internal_type::vec_boolean) {}
        explicit value(std::vector<bool> &&vec_boolean) : vec_boolean_(std::move(vec_boolean)), internal_type_(internal_type::vec_boolean) {}
        explicit value(const std::initializer_list<bool> &list_boolean) : vec_boolean_(list_boolean), internal_type_(internal_type::vec_boolean) {}

        value(const value &other);
        value(value &&other) noexcept;
        value &operator=(const value &other);
        value &operator=(value &&other) noexcept;
        ~value();

        bool operator==(const value &other) const;
        bool operator!=(const value &other) const;

        template<typename T>
        const T &get() const;

        internal_type get_internal_type() const { return internal_type_; }

        std::size_t num_elements() const;

    private:
        void copy(const value &other);
        void move(value &&other) noexcept;
        void destroy();

        union
        {
            std::vector<variant> vec_variant_;
            std::int8_t i1_;
            std::vector<std::int8_t> vec_i1_;
            std::int16_t i2_;
            std::vector<std::int16_t> vec_i2_;
            std::int32_t i4_;
            std::vector<std::int32_t> vec_i4_;
            std::int64_t i8_;
            std::vector<std::int64_t> vec_i8_;
            std::uint8_t ui1_;
            std::vector<std::uint8_t> vec_ui1_;
            std::uint16_t ui2_;
            std::vector<std::uint16_t> vec_ui2_;
            std::uint32_t ui4_;
            std::vector<std::uint32_t> vec_ui4_;
            std::uint64_t ui8_;
            std::vector<std::uint64_t> vec_ui8_;
            float r4_;
            std::vector<float> vec_r4_;
            double r8_;
            std::vector<double> vec_r8_;
            std::string str_;
            std::vector<std::string> vec_str_;
            xlnt::datetime datetime_;
            std::vector<xlnt::datetime> vec_datetime_;
            bool boolean_;
            std::vector<bool> vec_boolean_;
        };
        internal_type internal_type_ = internal_type::none;
    };

    value value_;
    type type_;
    base_type base_type_; // relevant for vectors and arrays
};

/// <summary>
/// Returns the value of this variant as type std::int8_t.
/// Assumes that the variant is of type::i1 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::int8_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::int8_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::i1 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::int8_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::int16_t.
/// Assumes that the variant is of type::i2 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::int16_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::int16_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::i2 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::int16_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::int32_t.
/// Assumes that the variant is of type::i4 or type::integer (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::int32_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::int32_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::i4 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::int32_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::int64_t.
/// Assumes that the variant is of type::i8 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::int64_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::int64_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::i8 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::int64_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::uint8_t.
/// Assumes that the variant is of type::ui1 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::uint8_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::uint8_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::ui1 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::uint8_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::uint16_t.
/// Assumes that the variant is of type::ui2 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::uint16_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::uint16_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::ui2 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::uint16_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::uint32_t.
/// Assumes that the variant is of type::ui4 or type::uint (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::uint32_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::uint32_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::ui4 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::uint32_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::uint64_t.
/// Assumes that the variant is of type::ui8 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::uint64_t variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::uint64_t>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::ui8 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::uint64_t> variant::get() const;

/// <summary>
/// Returns the value of this variant as type float.
/// Assumes that the variant is of type::r4 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API float variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<float>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::r4 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<float> variant::get() const;

/// <summary>
/// Returns the value of this variant as type double.
/// Assumes that the variant is of type::r8 (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API double variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<float>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::r8 (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<double> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::string.
/// Please call is() or value_type() to check the type. The following types are compatible:
/// - type::lpstr
/// - type::lpwstr
/// - type::bstr
/// - type::date (DEPRECATED, will no longer be allowed in a future major version)
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::string variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<std::string>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type (please call value_base_type() to check):
/// - base_type::lpstr
/// - base_type::lpwstr
/// - base_type::bstr
/// - base_type::date (DEPRECATED, will no longer be allowed in a future major version)
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<std::string> variant::get() const;

/// <summary>
/// Returns the value of this variant as type xlnt::datetime.
/// Assumes that the variant is of type::date or type::filetime (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API datetime variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<xlnt::datetime>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::date or base_type::filetime (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<datetime> variant::get() const;

/// <summary>
/// Returns the value of this variant as type bool.
/// Assumes that the variant is of type::boolean (please call is() or value_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API bool variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<bool>.
/// Assumes that the variant is of type::vector (please call is() or value_type() to check)
/// containing elements of base_type::boolean (please call value_base_type() to check).
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<bool> variant::get() const;

/// <summary>
/// Returns the value of this variant as type std::vector<xlnt::variant>.
/// Please call is() or value_type() to check the type, and value_base_type() to check the base type.
/// The following types are compatible:
/// - type::vector containing elements of base_type::variant
/// - type::vector containing elements of types: std::int32_t, std::string, xlnt::datetime, bool (DEPRECATED, will no longer be allowed in a future major version)
/// An xlnt::bad_variant_access exception will be thrown if the types are not compatible.
/// </summary>
template <>
XLNT_API std::vector<variant> variant::get() const;

} // namespace xlnt
