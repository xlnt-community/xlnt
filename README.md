<img height="100" src="https://user-images.githubusercontent.com/1735211/29433390-f37fa28e-836c-11e7-8a60-f8df4c30b424.png" alt="xlnt logo"><br/>
====

[![CircleCI Build status](https://dl.circleci.com/status-badge/img/gh/xlnt-community/xlnt/tree/master.svg?style=shield)](https://dl.circleci.com/status-badge/redirect/gh/xlnt-community/xlnt/tree/master)
[![Coverage Status](https://coveralls.io/repos/github/xlnt-community/xlnt/badge.svg?branch=master)](https://coveralls.io/github/xlnt-community/xlnt?branch=master)
[![Documentation](https://img.shields.io/badge/view-Documentation-blue)](https://xlnt-community.gitbook.io/xlnt)
[![API reference](https://img.shields.io/badge/view-API_reference-blue)](https://xlnt-community.github.io/xlnt/annotated.html)
[![License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://opensource.org/licenses/MIT)

## Introduction
xlnt is a modern C++ library (requiring c++11 or above) for manipulating spreadsheets in memory and reading/writing them from/to XLSX (Microsoft Excel®) files as described in [ECMA-376 5th edition](https://ecma-international.org/publications-and-standards/standards/ecma-376/). The first public release of xlnt version 1.0 was on May 10th, 2017. Current work is focused on increasing compatibility, improving performance, and brainstorming future development goals. For a high-level summary of what you can do with this library, see [the feature list](https://xlnt-community.gitbook.io/xlnt/introduction/features). Contributions are welcome in the form of pull requests or discussions on [the repository's Issues page](https://github.com/xlnt-community/xlnt/issues).

## About this fork
This repo is a community effort to continue the development of xlnt, after the [original repo of tfussel](https://github.com/tfussell/xlnt) has been unmaintained for many years (see [Issue #748](https://github.com/tfussell/xlnt/issues/748)).
The [xlnt community edition](https://github.com/xlnt-community/xlnt) is hosted at GitHub.
Feel free to participate in this community effort by submitting [issues](https://github.com/xlnt-community/xlnt/issues) and [PRs](https://github.com/xlnt-community/xlnt/pulls) to this new community-driven repo.
Issues and PRs on the original repo will not be transferred in bulk to this repo, but you may consider creating a similar issue or PR against this repo for items of interest to you.

## String encoding
XLNT generally expects strings to be encoded as UTF-8. This is **required** when saving files created by XLNT, which will fail when using special characters not encoded as UTF-8. This is an issue on compilers and IDEs that do not use UTF-8 by default, like Microsoft Visual Studio. There are a few things to keep in mind:

- For string literals (like `"test"`) written in the source code, you will need to make sure, at a minimum, that the execution character set for strings passed to XLNT is UTF-8:
    - Using `u8` string literals like `u8"test"` (available since C++11) will ensure that these strings are encoded as UTF-8 during compilation. However, since C++20, `u8` string literals need to be used with `std::u8string` or `std::u8string_view`, which XLNT currently supports only for paths. To use `u8` string literals with C++20 and newer while keeping compatibility with `std::string`, please use the following helpers that do not change the encoding or perform any conversions:
        - **`XLNT_U8`** for string literals, which provides the same behavior as `u8` did in C++11, C++14 and C++17
        - **`xlnt::to_string`** for copying **`std::u8string`**, **`std::u8string_view`** or **`const char8_t*`** to **`std::string`**
        - **`xlnt::to_string_view`** for casting **`std::u8string_view`** or **`const char8_t*`** to **`std::string_view`**
        - **`xlnt::to_char_ptr`** for casting **`const char8_t*`** to **`const char*`** and string literals like **`u8"test"`** to **`"test"`**
    - Alternatively, the execution character set can be changed in the compiler settings to force all string literals like `"test"` to be encoded as UTF-8 during compilation. For Visual Studio, compile using [`/execution-charset:utf-8`](https://learn.microsoft.com/en-us/cpp/build/reference/execution-charset-set-execution-character-set).
    - Optionally, the best solution would be to use UTF-8 for both the source and execution character set. To do this:
        1. Convert existing source code files to UTF-8, if they already contain special characters (outside of US-ASCII).
        2. Configure the IDE / editor to save future files as UTF-8. For Visual Studio, use [this solution](https://stackoverflow.com/a/65945041) to set this project-wide.
        3. Configure the compiler to use UTF-8. For Visual Studio, compile using [`/utf-8`](https://learn.microsoft.com/en-us/cpp/build/reference/utf-8-set-source-and-executable-character-sets-to-utf-8).
- For locale-aware formatting functions that can produce special characters, like string processing/formatting of the C and C++ Standard Libraries or of the Windows API, the above steps will not be enough if you want to pass such strings to XLNT. You will also need to ensure that the character encoding (active code page) is UTF-8 on all computers that run your application. Note that all major operating systems use UTF-8 by default nowadays, with a notable exception being Windows.
    - Windows only supports UTF-8 as the active code page since Windows 10 1903 (May 2019 Update) and Windows Server 2022, and is not enabled by default. The easiest is to [enforce UTF-8 code page using a manifest](https://learn.microsoft.com/en-us/windows/apps/design/globalizing/use-utf8-code-page) for your application.
    - For all other cases and operating systems, setting `std::locale::global` and/or `setlocale` to a UTF-8 encoding should work if the operating system supports UTF-8 character encodings.
- An alternative to all of the steps above is to use a Unicode library and convert all strings to UTF-8 before passing them to XLNT. However, this is slower and needs more code (but works for all operating systems), so try to avoid this when possible.
- For strings coming from external sources (e.g. files, databases, APIs), you will need to ensure that they always return UTF-8 encoded strings, or convert them to UTF-8 using a Unicode library before passing such strings to XLNT.

For more details, please see [issue #134](https://github.com/xlnt-community/xlnt/issues/134).

## Example

Including xlnt in your project, creating a new spreadsheet, and saving it as "example.xlsx"

```c++
#include <xlnt/xlnt.hpp>

int main()
{
    xlnt::workbook wb;
    xlnt::worksheet ws = wb.active_sheet();
    ws.cell("A1").value(5);
    ws.cell("B2").value("string data");
    ws.cell("C3").formula("=RAND()");
    ws.merge_cells("C3:C4");
    ws.freeze_panes("B2");
    wb.save("example.xlsx");
    return 0;
}
// compile with -std=c++11 -Ixlnt/include -lxlnt
```

[More examples](https://xlnt-community.gitbook.io/xlnt/introduction/examples) are available.

## Documentation

More information is available in the [xlnt documentation](https://xlnt-community.gitbook.io/xlnt/) and in the [xlnt API reference](https://xlnt-community.github.io/xlnt/annotated.html).

## Building xlnt - From source

You can download [the xlnt source code](https://github.com/xlnt-community/xlnt) and install the latest xlnt version as follows:

    git clone https://github.com/xlnt-community/xlnt.git xlnt --recurse-submodules
    cd xlnt
    mkdir build
    cd build
    cmake ..
    cmake --build . -j 4
    cmake --install .

For more information, see the [full installation instructions](https://xlnt-community.gitbook.io/xlnt/introduction/installation#compiling-from-source).

## Building xlnt - Using vcpkg

You can download and install xlnt using the [vcpkg](https://github.com/microsoft/vcpkg) dependency manager:

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install xlnt

The [xlnt port in vcpkg](https://vcpkg.io/en/package/xlnt) is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/microsoft/vcpkg) on the vcpkg repository.

## License
xlnt is released to the public for free under the terms of the MIT License. See [LICENSE.md](https://github.com/xlnt-community/xlnt/blob/master/LICENSE.md) for the full text of the license and the licenses of xlnt's third-party dependencies. [LICENSE.md](https://github.com/xlnt-community/xlnt/blob/master/LICENSE.md) should be distributed alongside any assemblies that use xlnt in source or compiled form.
