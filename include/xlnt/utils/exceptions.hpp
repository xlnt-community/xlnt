// Copyright (c) 2014-2022 Thomas Fussell
// Copyright (c) 2010-2015 openpyxl
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

#pragma once

#include <stdexcept>

#include <xlnt/xlnt_config.hpp>
#include <xlnt/cell/index_types.hpp>

namespace xlnt {

/// <summary>
/// Parent type of all custom exceptions thrown in this library.
/// </summary>
class XLNT_API exception : public std::runtime_error
{
public:
    /// <summary>
    /// Constructs an exception with a message. This message will be
    /// returned by std::exception::what(), an inherited member of this class.
    /// </summary>
    explicit exception(const std::string &message);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    exception(const exception &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~exception() override;

    /// <summary>
    /// Sets the message after the xlnt::exception is constructed. This can show
    /// more specific information than std::exception::what().
    /// </summary>
    void message(const std::string &message);

    /// <summary>
    /// Gets the message containing extra information.
    /// </summary>
    const std::string & message();

private:
    /// <summary>
    /// The exception message
    /// </summary>
    std::string message_;
};

/// <summary>
/// Exception for a bad parameter value
/// </summary>
class XLNT_API invalid_parameter : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    explicit invalid_parameter(const char *optional_message = nullptr);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_parameter(const invalid_parameter &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_parameter() override;
};

/// <summary>
/// Exception for bad sheet names.
/// </summary>
class XLNT_API invalid_sheet_title : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    explicit invalid_sheet_title(const std::string &title);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_sheet_title(const invalid_sheet_title &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_sheet_title() override;
};

/// <summary>
/// Exception for trying to open a non-XLSX file.
/// </summary>
class XLNT_API invalid_file : public exception
{
public:
    /// <summary>
    /// Constructs an invalid_file exception thrown when attempt to access
    /// the given filename.
    /// </summary>
    explicit invalid_file(const std::string &filename);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_file(const invalid_file &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_file() override;
};

/// <summary>
/// The data submitted which cannot be used directly in Excel files. It
/// must be removed or escaped.
/// </summary>
class XLNT_API illegal_character : public exception
{
public:
    /// <summary>
    /// Constructs an illegal_character exception thrown as a result of the given character.
    /// </summary>
    explicit illegal_character(char c);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    illegal_character(const illegal_character &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~illegal_character() override;
};

/// <summary>
/// Exception for any data type inconsistencies.
/// </summary>
class XLNT_API invalid_data_type : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    invalid_data_type();

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_data_type(const invalid_data_type &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_data_type() override;
};

/// <summary>
/// Exception for bad column indices in A1-style cell references.
/// </summary>
class XLNT_API invalid_column_index : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    invalid_column_index();

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_column_index(const invalid_column_index &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_column_index() override;
};

/// <summary>
/// Exception for converting between numeric and A1-style cell references.
/// </summary>
class XLNT_API invalid_cell_reference : public exception
{
public:
    /// <summary>
    /// Constructs an invalid_cell_reference exception for the given column and row.
    /// </summary>
    invalid_cell_reference(column_t column, row_t row);

    /// <summary>
    /// Constructs an invalid_cell_reference exception for the given string.
    /// </summary>
    explicit invalid_cell_reference(const std::string &reference_string);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_cell_reference(const invalid_cell_reference &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_cell_reference() override;
};

/// <summary>
/// Exception when setting a class's attribute to an invalid value
/// </summary>
class XLNT_API invalid_attribute : public exception
{
public:
    /// <summary>
    /// Constructor.
    /// </summary>
    explicit invalid_attribute(const char *optional_message = nullptr);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    invalid_attribute(const invalid_attribute &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~invalid_attribute() override;
};

/// <summary>
/// Exception for a key that doesn't exist in a container
/// </summary>
class XLNT_API key_not_found : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    key_not_found();

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    key_not_found(const key_not_found &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~key_not_found() override;
};

/// <summary>
/// Exception for a workbook with no visible worksheets
/// </summary>
class XLNT_API no_visible_worksheets : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    no_visible_worksheets();

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    no_visible_worksheets(const no_visible_worksheets &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~no_visible_worksheets() override;
};

/// <summary>
/// Debug exception for a switch that fell through to the default case
/// </summary>
class XLNT_API unhandled_switch_case : public exception
{
public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    unhandled_switch_case();

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    unhandled_switch_case(const unhandled_switch_case &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~unhandled_switch_case() override;
};

/// <summary>
/// Exception for attempting to use a feature which is not supported
/// </summary>
class XLNT_API unsupported : public exception
{
public:
    /// <summary>
    /// Constructs an unsupported exception with a message describing the unsupported
    /// feature.
    /// </summary>
    explicit unsupported(const std::string &message);

    /// <summary>
    /// Default copy constructor.
    /// </summary>
    unsupported(const unsupported &) = default;

    /// <summary>
    /// Destructor
    /// </summary>
    ~unsupported() override;
};

} // namespace xlnt
