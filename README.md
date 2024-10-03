<img height="100" src="https://user-images.githubusercontent.com/1735211/29433390-f37fa28e-836c-11e7-8a60-f8df4c30b424.png" alt="xlnt logo"><br/>
====

[![CircleCI Build status](https://dl.circleci.com/status-badge/img/gh/xlnt-community/xlnt/tree/master.svg?style=shield)](https://dl.circleci.com/status-badge/redirect/gh/xlnt-community/xlnt/tree/master)
[![Coverage Status](https://coveralls.io/repos/github/xlnt-community/xlnt/badge.svg?branch=master)](https://coveralls.io/github/xlnt-community/xlnt?branch=master)
[![Documentation](https://img.shields.io/badge/view-Documentation-blue)](https://xlnt-community.gitbook.io/xlnt)
[![API reference](https://img.shields.io/badge/view-API_reference-blue)](https://xlnt-community.github.io/xlnt/annotated.html)
[![License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://opensource.org/licenses/MIT)

## Introduction
xlnt is a modern C++ library for manipulating spreadsheets in memory and reading/writing them from/to XLSX files as described in [ECMA 376 5th edition](https://ecma-international.org/publications-and-standards/standards/ecma-376/). The first public release of xlnt version 1.0 was on May 10th, 2017. Current work is focused on increasing compatibility, improving performance, and brainstorming future development goals. For a high-level summary of what you can do with this library, see [the feature list](https://xlnt-community.gitbook.io/xlnt/introduction/features). Contributions are welcome in the form of pull requests or discussions on [the repository's Issues page](https://github.com/xlnt-community/xlnt/issues).

## About this fork
This repo is a community effort to continue the development of xlnt, after the [original repo of tfussel](https://github.com/tfussell/xlnt) has been unmaintained for many years (see [Issue #748](https://github.com/tfussell/xlnt/issues/748)).
Feel free to participate in this community effort by submitting issues and PRs to this new community-driven repo.
Issues and PRs on the original repo will not be transferred in bulk to this repo, but you may consider creating a similar issue or PR against this repo for items of interest to you.

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
// compile with -std=c++14 -Ixlnt/include -lxlnt
```

More examples can be found [here](https://xlnt-community.gitbook.io/xlnt/introduction/examples).

## Documentation

Documentation for the current release of xlnt is available [here](https://xlnt-community.gitbook.io/xlnt/).

The latest API reference can be found [here](https://xlnt-community.github.io/xlnt/annotated.html).

## Building xlnt - From source

You can download and install the latest xlnt version as follows:

    git clone https://github.com/xlnt-community/xlnt.git xlnt --recurse-submodules
    cd xlnt
    mkdir build
    cd build
    cmake ..
    cmake --build . -j 4
    cmake --install .

Full installation instructions can be found [here](https://xlnt-community.gitbook.io/xlnt/introduction/installation#compiling-from-source).

## Building xlnt - Using vcpkg

You can download and install xlnt using the [vcpkg](https://github.com/microsoft/vcpkg) dependency manager:

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install xlnt

The xlnt port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/microsoft/vcpkg) on the vcpkg repository.

## License
xlnt is released to the public for free under the terms of the MIT License. See [LICENSE.md](https://github.com/xlnt-community/xlnt/blob/master/LICENSE.md) for the full text of the license and the licenses of xlnt's third-party dependencies. [LICENSE.md](https://github.com/xlnt-community/xlnt/blob/master/LICENSE.md) should be distributed alongside any assemblies that use xlnt in source or compiled form.
