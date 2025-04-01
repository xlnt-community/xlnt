# Getting xlnt

The most up to date and stable version can be obtained by building xlnt from source (using the master branch).

## Binaries

### Arch

> [!WARNING]
> Provided binary (v1.5.0) is rather old, it may be better to build from source or use vcpkg.

xlnt can be [found](https://aur.archlinux.org/packages/xlnt/) on the AUR.

### vcpkg
`vcpkg` installs x86 by default
```
.\vcpkg install xlnt
```
if you need x64 use the following command
```
.\vcpkg install xlnt:x64-windows
```

## Compiling from source

The latest xlnt version can be obtained by building xlnt from source (using the git master branch). All changes are reviewed and tested using CI before being added to the master branch.

Build configurations for Visual Studio, GNU Make, Ninja, and Xcode can be created using [cmake](https://cmake.org/) v3.2+. A full list of cmake generators can be found [here](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html). A basic build would look like (starting in the root xlnt directory):

### General

1. Install dependencies:

To build xlnt from source, you should install git, CMake and a C++ compiler.

2. Obtain the source files:

Latest source files can be obtained using git:
```
git clone https://github.com/xlnt-community/xlnt.git xlnt --recurse-submodules
```

The `--recurse-submodules` option is needed to download the third-party libstudxml dependency. If the repo was cloned without this option, libstudxml can be downloaded using the following command:
```
git submodule update --init --recursive
```

If you later want to update the source files to the latest version, you can run the following command:
```
git pull
git submodule update --init --recursive
```

3. Create build configuration

Build configuration is generated using CMake:

```
cd xlnt
mkdir build
cd build
cmake ..
```

By default, xlnt is built as a shared library. Some important options are:

 - STATIC: Set to ON to build xlnt as a static library instead of a shared library.
 - TESTS: Set to ON to build test executable (in ./tests).
 - DOCUMENTATION: Set to ON to build API reference documentation (in ./api-reference). Requires doxygen being installed on your system.
 - CMAKE_INSTALL_PREFIX: specify the location where you want to install xlnt.
 - CMAKE_BUILD_TYPE: Specify the desired build type (Debug, Release, RelWithDebInfo or MinSizeRel). Only applicable for single-configuration generators (Makefile or Ninja), typically used on Linux derivatives (e.g. Ubuntu).
 
To build a static library including tests and documentation, you can use the following cmake command:

```
cmake -D STATIC=ON -D TESTS=ON -D DOCUMENTATION=ON ..
```
 
Other CMake configuration options for xlnt can be found using "cmake -LH".
 
4. Build

```
cmake --build . -j 4
```

In case of a multi-configuration generators (e.g. Visual Studio and Xcode), you can specify the desired build type (e.g. `Debug` or `Release`):

```
cmake --build . -j 4 --config Release
```

5. Install

```
cmake --install .
```

In case of a multi-configuration generators (e.g. Visual Studio and Xcode), it may be needed to specify the build type again (e.g. `Debug` or `Release`):

```
cmake --install . --config Release
```

### Example: Ubuntu 24.04 LTS (Noble Numbat) 
Time required: Approximately 5 minutes (depending on your internet speed)

Install dependencies (CMake, C++ compiler):
```
sudo apt-get install cmake
sudo apt-get install build-essential
```

The following steps will install xlnt
```
git clone https://github.com/xlnt-community/xlnt.git xlnt --recurse-submodules
cd xlnt
mkdir build
cd build
cmake ..
make -j 4
sudo make install
```

The following step will map the shared library names to the location of the corresponding shared library files
```
sudo ldconfig
```
xlnt will now be ready to use on your Ubuntu instance. 

### Example: Xcode

Install git, CMake and Xcode.

Download the source code:
```
git clone https://github.com/xlnt-community/xlnt.git xlnt --recurse-submodules
```

Generate the Xcode project:
```
cd xlnt
mkdir build
cd build
cmake -G Xcode ..
```

Build the project:
```bash
cmake --build . -j 4 --config Release
```

The resulting shared (e.g. libxlnt.dylib) library would be found in the build/lib directory.

### Example: Windows

Install git, CMake and Visual Studio.

Download the source code:
```
git clone https://github.com/xlnt-community/xlnt.git xlnt --recurse-submodules
```

Generate the Visual Studio project file:
```
cd xlnt
mkdir build
cd build
cmake ..
```

If desired, you can specify the Visual Studio version and architecture to use. To build a 64-bit library using Visual Studio 2019, use the following command:
```bash
cmake -G "Visual Studio 16 2019" -A x64 ..
```

Open the generated solution file `xlnt_all.sln` (inside the build folder) using Visual Studio and build the `INSTALL` project (or the `ALL_BUILD` project if you don't want the install the library). The project will by default be installed in the `installed` subdirectory of the build folder.

## Using xlnt in your project

### General

1. Use at least c++11 (e.g. -std=c++14)

2. Include `xlnt/xlnt.hpp` in your source file:
```c++
#include <xlnt/xlnt.hpp>
```

If xlnt is not installed in a default location, you may need to specify the include directory where the xlnt files may be found (e.g. -Ixlnt/include)

3. Link against the libxlnt library (e.g. -lxlnt).

See our [REAMDE](https://xlnt-community.gitbook.io/xlnt#example) or [Basic examples](https://xlnt-community.gitbook.io/xlnt/introduction/examples) to get started with using xlnt.

4. Ensure the xlnt library is in your (library) path (if xlnt is built as a shared library and installed in a non-default location or on Windows):

 - On unix: `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/custom/xlnt/install/directory`
 - On Windows: `set PATH=%PATH%;c:\path\to\custom\xlnt\install\directory`

### Example: CMake project

Add the following lines to your CMakeLists.txt to use xlnt into your project:
```
find_package(Xlnt)
target_link_libraries(<target> PRIVATE xlnt::xlnt)
```
with `<target>` being replaced by the name of your target (see `add_executable`).

If CMake could not find Xlnt (e.g. because you are building on Windows or installed xlnt in a non-default location, see CMAKE_INSTALL_PREFIX), you should specify the path where xlnt is installed:
```
list(APPEND CMAKE_PREFIX_PATH "/path/to/xlnt/install/prefix")
```

When running, you should still make sure that the xlnt library is in your (library) path (if xlnt is built as a shared library): see the general instructions.
