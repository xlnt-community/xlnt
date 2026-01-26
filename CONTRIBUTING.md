# Contributing to xlnt

xlnt welcomes contributions from everyone regardless of skill level (provided you can write C++ or documentation).

## Getting Started

Look through the list of issues to find something interesting to work on. Help is appreciated with any issues, but important timely issues are labeled as "help wanted". Issues labeled "docs" might be good for those who want to contribute without having to know too much C++. You might also find something that the code is missing without an associated issue. That's fine to work on to, but it might be best to make an issue first in case someone else is working on it.

The XLSX format is described in [ECMA-376 5th edition](https://ecma-international.org/publications-and-standards/standards/ecma-376/). Part 1 contains the most relevant information:

 - A PDF with all information, especially section "18. SpreadsheetML Reference Material" may be useful. Using the outline of this section allows you to navigate to all the relevant parts of the XLSX format.
 - OfficeOpenXml-XMLSCHEMA-Strict.zip > sml.xsd: contains the xsd specification of the XLSX format. This information is also available in the PDF in annex A.2 SpreadsheetML.
 
There are some further specifications that are not part of ECMA-376, but are relevant for specific features:

 - [.ZIP File Format Specification from PKWARE, Inc., version 6.2.0 (2004)](http://www.pkware.com/documents/APPNOTE/APPNOTE_6.2.0.txt) for zipping and unzipping the contents of XLSX files.
 - [[MS-OFFCRYPTO]: Office Document Cryptography Structure](https://learn.microsoft.com/en-us/openspecs/office_file_formats/ms-offcrypto/3c34d72a-1a61-4b52-a893-196f9157f083), for encrypted XLSX files.
 - [[MS-CFB]: Compound File Binary File Format](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-cfb/53989ce4-7b05-4f8d-829b-d08d6148375b), used by encrypted XLSX files and older XLS files.
 - [[MS-OE376]: Office Implementation Information for ECMA-376 Standards Support](https://learn.microsoft.com/en-us/openspecs/office_standards/ms-oe376/db9b9b72-b10b-4e7e-844c-09f88c972219), for Microsoft Excel-specific implementation details not standardized by ECMA-376.
 - [[MS-ODRAWXML]: Office Drawing Extensions to Office Open XML Structure](https://learn.microsoft.com/en-us/openspecs/office_standards/ms-odrawxml/06cff208-c6e1-4db7-bb68-665135e5f0de), for drawings.
 - [[MS-OFFMACRO]: Office Macro-Enabled File Format](https://learn.microsoft.com/en-us/openspecs/office_standards/ms-offmacro/86fa5ba8-69cf-4164-8559-c5af66d2990e), for macro-enabled XLSM files.
 - [[MS-OFFMACRO2]: Office Macro-Enabled File Format Version 2](https://learn.microsoft.com/en-us/openspecs/office_standards/ms-offmacro2/802a7c98-c802-41c6-8a13-987457098d8f), for macro-enabled XLSM files.

Some further specifications that are not implemented, in case anyone wants to contribute them to XLNT:

 - ISO/IEC 29500, the ISO and IEC standardized version of ECMA-376. Exists in two versions:
   - ISO/IEC 29500 Transitional, fully supported since Microsoft Excel 2010
   - ISO/IEC 29500 Strict, supported for reading since Microsoft Excel 2010, and fully supported since Microsoft Excel 2013
 - [[MS-XLSX]: Excel (.xlsx) Extensions to the Office Open XML SpreadsheetML File Format](https://learn.microsoft.com/en-us/openspecs/office_standards/ms-xlsx/2c5dee00-eff2-4b22-92b6-0738acd4475e), specifies extensions to
 the Office Open XML file formats described in ISO/IEC 29500.
 - [[MS-OI29500]: Office Implementation Information for ISO/IEC 29500 Standards Support](https://learn.microsoft.com/en-us/openspecs/office_standards/ms-oi29500/1fd4a662-8623-49c0-82f0-18fa91b413b8), for Microsoft Excel-specific implementation details not standardized by ISO/IEC 29500.
 - [[MS-XLS]: Excel Binary File Format (.xls) Structure](https://learn.microsoft.com/en-us/openspecs/office_file_formats/ms-xls/cd03cb5f-ca02-4934-a391-bb674cb8aa06), for the older XLS format used by default in Microsoft Excel 2003 and earlier. Also see [tfussell/xlnt#227](https://github.com/tfussell/xlnt/issues/227) for more details.
 - [[MS-XLSB]: Excel (.xlsb) Binary File Format](https://learn.microsoft.com/en-us/openspecs/office_file_formats/ms-xlsb/acc8aa92-1f02-4167-99f5-84f9f676b95a), for XLSB files, supported since Microsoft Excel 2007.

## Contributions

Contributions to xlnt should be made in the form of pull requests on GitHub. Each pull request will be reviewed and either merged into the current development branch or given feedback for changes that would be required to do so. 

All code in this repository is under the MIT License. You should agree to these terms before submitting any code to xlnt.

## Pull Request Checklist

- Branch from the head of the current development branch. Until version 1.0 is released, this the master branch.

- Commits should be as small as possible, while ensuring that each commit is correct independently (i.e. each commit should compile and pass all tests). Commits that don't follow the coding style indicated in .clang-format (e.g. indentation) are less likely to be accepted until they are fixed.

- If your pull request is not getting reviewed or you need a specific person to review it, you can @-reply a reviewer asking for a review in the pull request or a comment.

- Add tests relevant to the fixed defect or new feature. It's best to do this before making any changes, make sure that the tests fail, then make changes ensuring that it ultimately passes the tests (i.e. TDD). xlnt uses cxxtest for testing. Tests are contained in a tests directory inside each module (e.g. source/workbook/tests/test_workbook.hpp) in the form of a header file. Each test is a separate function with a name that starts like "test_". See http://cxxtest.com/guide.html for information about CxxTest or take a look at existing tests.

## Conduct

Just try to be nice--we're all volunteers here.

## Communication

Add a comment to an existing issue on GitHub, open a new issue for defects or feature requests, or contact @m7913d or @doomlaur if you want.
