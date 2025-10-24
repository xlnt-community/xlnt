cmake_minimum_required(VERSION 3.1...3.31)
project(xlnt VERSION 1.6.1)

set(CMAKE_CXX_STANDARD ${XLNT_CXX_LANG})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD ${XLNT_C_LANG})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

# Project metadata
set(PROJECT_VENDOR "xlnt-community")
set(PROJECT_CONTACT "https://github.com/xlnt-community/xlnt/issues")
set(PROJECT_URL "https://github.com/xlnt-community/xlnt")
set(PROJECT_DESCRIPTION "cross-platform user-friendly xlsx library for C++11")

# Commonly used project directories
set(XLNT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(XLNT_INCLUDE_DIR ${XLNT_ROOT_DIR}/include)
set(XLNT_SOURCE_DIR ${XLNT_ROOT_DIR}/source)
set(THIRD_PARTY_DIR ${XLNT_ROOT_DIR}/third-party)

# Include libstudxml library
add_subdirectory(${THIRD_PARTY_DIR}/libstudxml.build
  ${CMAKE_CURRENT_BINARY_DIR}/third-party/libstudxml)

if(COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Non-target-specific compiler settings
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4") # level 4 warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # multi-processor compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4324") # structure was padded due to alignment specifier
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra") # extra warnings
  if (XLNT_ALL_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") # avoid code being pushed with warnings
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas") # ignore MSVC and Clang pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized") # GCC diagnostic with lots of false positives
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything") # all warnings
  # blacklist warnings that are not relevant
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat") # ignore warnings about C++98 compatibility
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat-pedantic") # ignore pedantic warnings about C++98 compatibility
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-padded") # ignore padding warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-documentation-unknown-command") # ignore unknown commands in Javadoc-style comments
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas") # ignore Windows and GCC pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option") # ignore Windows and GCC pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-newline-eof") # no longer an issue with post-c++11 standards which mandate include add a newline if neccesary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-covered-switch-default") # default is often added to switches for completeness or to cover future alternatives
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-exit-time-destructors") # this is just a warning to notify that the destructor will run during exit
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-braces") # Wmissing-field-initializers has less false positives
endif()

if(STATIC_CRT)
  include(${XLNT_ROOT_DIR}/cmake/ucm.cmake)
  ucm_set_runtime(STATIC)
endif()

if(APPLE)
  # Prevent a warning about deployment target not being set by setting it to current OSX version
  execute_process(COMMAND "sw_vers -productVersion | awk -F'.' '{print $1\".\"$2}'"
    OUTPUT_VARIABLE OSX_VERSION)
  set(CMAKE_OSX_DEPLOYMENT_TARGET ${OSX_VERSION})
endif()

file(GLOB ROOT_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/*.hpp)
file(GLOB CELL_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/cell/*.hpp)
file(GLOB CELL_SOURCES ${XLNT_SOURCE_DIR}/cell/*.cpp)
file(GLOB DRAWING_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/drawing/*.hpp)
file(GLOB DRAWING_SOURCES ${XLNT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB CHARTS_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/charts/*.hpp)
file(GLOB CHARTS_SOURCES ${XLNT_SOURCE_DIR}/charts/*.cpp)
file(GLOB CHARTSHEET_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/chartsheet/*.hpp)
file(GLOB CHARTSHEET_SOURCES ${XLNT_SOURCE_DIR}/chartsheet/*.cpp)
file(GLOB DRAWING_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/drawing/*.hpp)
file(GLOB DRAWING_SOURCES ${XLNT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB FORMULA_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/formula/*.hpp)
file(GLOB FORMULA_SOURCES ${XLNT_SOURCE_DIR}/formula/*.cpp)
file(GLOB PACKAGING_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/packaging/*.hpp)
file(GLOB PACKAGING_SOURCES ${XLNT_SOURCE_DIR}/packaging/*.cpp)
file(GLOB STYLES_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/styles/*.hpp)
file(GLOB STYLES_SOURCES ${XLNT_SOURCE_DIR}/styles/*.cpp)
file(GLOB UTILS_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/utils/*.hpp)
file(GLOB UTILS_SOURCES ${XLNT_SOURCE_DIR}/utils/*.cpp)
file(GLOB WORKBOOK_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/workbook/*.hpp)
file(GLOB WORKBOOK_SOURCES ${XLNT_SOURCE_DIR}/workbook/*.cpp)
file(GLOB WORKSHEET_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/worksheet/*.hpp)
file(GLOB WORKSHEET_SOURCES ${XLNT_SOURCE_DIR}/worksheet/*.cpp)
file(GLOB MINIZ_HEADERS ${THIRD_PARTY_DIR}/miniz/*.h)
file(GLOB MINIZ_SOURCES ${THIRD_PARTY_DIR}/miniz/*.c)

file(GLOB DETAIL_ROOT_HEADERS ${XLNT_SOURCE_DIR}/detail/*.hpp)
file(GLOB DETAIL_ROOT_SOURCES ${XLNT_SOURCE_DIR}/detail/*.cpp)
file(GLOB DETAIL_CRYPTOGRAPHY_HEADERS ${XLNT_SOURCE_DIR}/detail/cryptography/*.hpp)
file(GLOB DETAIL_CRYPTOGRAPHY_SOURCES ${XLNT_SOURCE_DIR}/detail/cryptography/*.c*)
file(GLOB DETAIL_EXTERNAL_HEADERS ${XLNT_SOURCE_DIR}/detail/external/*.hpp)
file(GLOB DETAIL_HEADER_FOOTER_HEADERS ${XLNT_SOURCE_DIR}/detail/header_footer/*.hpp)
file(GLOB DETAIL_HEADER_FOOTER_SOURCES ${XLNT_SOURCE_DIR}/detail/header_footer/*.cpp)
file(GLOB DETAIL_IMPLEMENTATIONS_HEADERS ${XLNT_SOURCE_DIR}/detail/implementations/*.hpp)
file(GLOB DETAIL_IMPLEMENTATIONS_SOURCES ${XLNT_SOURCE_DIR}/detail/implementations/*.cpp)
file(GLOB DETAIL_NUMBER_FORMAT_HEADERS ${XLNT_SOURCE_DIR}/detail/number_format/*.hpp)
file(GLOB DETAIL_NUMBER_FORMAT_SOURCES ${XLNT_SOURCE_DIR}/detail/number_format/*.cpp)
file(GLOB DETAIL_SERIALIZATION_HEADERS ${XLNT_SOURCE_DIR}/detail/serialization/*.hpp)
file(GLOB DETAIL_SERIALIZATION_SOURCES ${XLNT_SOURCE_DIR}/detail/serialization/*.cpp)
file(GLOB DETAIL_UTILS_HEADERS ${XLNT_SOURCE_DIR}/detail/utils/*.hpp)
file(GLOB DETAIL_UTILS_SOURCES ${XLNT_SOURCE_DIR}/detail/utils/*.cpp)


set(DETAIL_HEADERS ${DETAIL_ROOT_HEADERS} ${DETAIL_CRYPTOGRAPHY_HEADERS}
  ${DETAIL_EXTERNAL_HEADERS} ${DETAIL_HEADER_FOOTER_HEADERS}
  ${DETAIL_IMPLEMENTATIONS_HEADERS} ${DETAIL_NUMBER_FORMAT_HEADERS}
  ${DETAIL_SERIALIZATION_HEADERS} ${DETAIL_UTILS_HEADERS})
set(DETAIL_SOURCES ${DETAIL_ROOT_SOURCES} ${DETAIL_CRYPTOGRAPHY_SOURCES}
  ${DETAIL_EXTERNAL_SOURCES} ${DETAIL_HEADER_FOOTER_SOURCES}
  ${DETAIL_IMPLEMENTATIONS_SOURCES} ${DETAIL_NUMBER_FORMAT_SOURCES}
  ${DETAIL_SERIALIZATION_SOURCES} ${DETAIL_UTILS_SOURCES})

set(XLNT_HEADERS ${ROOT_HEADERS} ${CELL_HEADERS} ${CHARTS_HEADERS}
  ${CHARTSHEET_HEADERS} ${DRAWING_HEADERS} ${FORMULA_HEADERS}
  ${PACKAGING_HEADERS} ${STYLES_HEADERS} ${UTILS_HEADERS}
  ${WORKBOOK_HEADERS} ${WORKSHEET_HEADERS} ${DETAIL_HEADERS} ${DETAIL_CRYPTO_HEADERS}
  ${DRAWING_HEADERS} ${MINIZ_HEADERS})
set(XLNT_SOURCES ${CELL_SOURCES} ${CHARTS_SOURCES} ${CHARTSHEET_SOURCES}
  ${DRAWING_SOURCES} ${FORMULA_SOURCES} ${PACKAGING_SOURCES}
  ${STYLES_SOURCES} ${UTILS_SOURCES} ${WORKBOOK_SOURCES}
  ${WORKSHEET_SOURCES} ${DETAIL_SOURCES} ${DETAIL_CRYPTO_SOURCES}
  ${DRAWING_SOURCES} ${MINIZ_SOURCES})

# ============================================================================
# ZIP Backend Configuration (MUST be before add_library to affect GLOB files)
# ============================================================================
# xlnt supports two ZIP compression backends:
#
# 1. "builtin" (default):
#    - Uses miniz library (embedded in xlnt)
#    - Lightweight, no external dependencies
#    - Does NOT support Zip64 (files > 4GB)
#    - Recommended for most use cases
#
# 2. "minizipng":
#    - Uses minizip-ng library with zlib-ng (auto-fetched via CMake)
#    - Full Zip64 support (handles large files and archives)
#    - Requires external dependencies (minizip-ng, zlib-ng)
#    - Recommended for large Excel files or advanced ZIP features
#
# To change the backend, set XLNT_ZIP_BACKEND to "builtin" or "minizipng"
# Example: cmake -DXLNT_ZIP_BACKEND=minizipng ..
# ============================================================================

set(XLNT_ZIP_BACKEND "builtin" CACHE STRING "ZIP backend: builtin or minizipng")
set_property(CACHE XLNT_ZIP_BACKEND PROPERTY STRINGS builtin minizipng)

if(XLNT_ZIP_BACKEND STREQUAL "minizipng")
    message(STATUS "Using minizip-ng ZIP backend (Zip64 support enabled)")

    # Configure minizip-ng to fetch and build zlib-ng automatically
    set(MZ_COMPAT OFF CACHE BOOL "Build minizip 1.x compatibility layer" FORCE)
    set(MZ_ZLIB ON CACHE BOOL "Enable ZLIB compression" FORCE)
    set(MZ_BZIP2 OFF CACHE BOOL "Enable BZIP2 compression" FORCE)
    set(MZ_LZMA OFF CACHE BOOL "Enable LZMA compression" FORCE)
    set(MZ_ZSTD OFF CACHE BOOL "Enable ZSTD compression" FORCE)
    set(MZ_OPENSSL OFF CACHE BOOL "Enable OpenSSL for encryption" FORCE)
    set(MZ_LIBBSD OFF CACHE BOOL "Use libbsd" FORCE)
    set(MZ_FETCH_LIBS ON CACHE BOOL "Let minizip-ng fetch zlib-ng automatically" FORCE)
    set(MZ_BUILD_TESTS OFF CACHE BOOL "Build minizip-ng tests" FORCE)
    set(MZ_BUILD_UNIT_TESTS OFF CACHE BOOL "Build minizip-ng unit tests" FORCE)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third-party/minizip-ng
                     ${CMAKE_BINARY_DIR}/third-party/minizip-ng
                     EXCLUDE_FROM_ALL)

    # Set compile definition as a directory property so GLOB files get it
    add_compile_definitions(XLNT_USE_MINIZIP_NG)

    message(STATUS "minizip-ng backend configured (with auto-fetched zlib-ng)")
else()
    message(STATUS "Using builtin ZIP backend (miniz, no Zip64 support)")
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  # Set a default CMAKE_INSTALL_PREFIX if one wasn't specified

  if(MSVC)
    # No good place to install on Windows so just install to cmake build directory in /installed
    set(DEFAULT_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/installed")
  else()
    # Install to /usr/local on other platforms
    set(DEFAULT_INSTALL_PREFIX "/usr/local")
  endif()

  # Apply the variable and save to cache
  set(CMAKE_INSTALL_PREFIX ${DEFAULT_INSTALL_PREFIX}
    CACHE PATH "default install path" FORCE)
endif()

# Append "d" to the name of the compiled library
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set debug library postfix")

include(GNUInstallDirs)

set(XLNT_INC_DEST_DIR ${CMAKE_INSTALL_INCLUDEDIR}
	CACHE PATH "Default location to install include files")
set(XLNT_LIB_DEST_DIR ${CMAKE_INSTALL_LIBDIR}
	CACHE PATH "Default location to install library files")
set(XLNT_BIN_DEST_DIR ${CMAKE_INSTALL_BINDIR}
	CACHE PATH "Default location to install runtime files")
set(XLNT_MAN_DEST_DIR ${CMAKE_INSTALL_MANDIR}
	CACHE PATH "Default location to install runtime files")
set(XLNT_CMAKE_CFG_DEST_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	CACHE PATH "Default location to install CMake config files")

if(NOT STATIC)
  # Compile shared library
  add_library(xlnt SHARED
    ${XLNT_HEADERS}
    ${XLNT_SOURCES}
    $<TARGET_OBJECTS:libstudxml>)

  target_compile_definitions(xlnt PRIVATE XLNT_SHARED=1)

  # Set SO version (for symbolic links like libxlnt.so.1.4 and libxlnt.so.1)
  set_target_properties(xlnt
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    INSTALL_NAME_DIR "${XLNT_LIB_DEST_DIR}")
else()
  # Compile static library
  add_library(xlnt STATIC ${XLNT_HEADERS} ${XLNT_SOURCES} $<TARGET_OBJECTS:libstudxml>)
  target_compile_definitions(xlnt PUBLIC XLNT_STATIC=1)
endif()

# hide all symbols by default
set_target_properties(xlnt PROPERTIES CXX_VISIBILITY_PRESET hidden)

# generate XLNT_API and XLNT_DEPRECATED
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER (xlnt
    BASE_NAME xlnt_cmake
    EXPORT_MACRO_NAME XLNT_API
    DEPRECATED_MACRO_NAME XLNT_DEPRECATED
    EXPORT_FILE_NAME ${XLNT_INCLUDE_DIR}/xlnt/utils/xlnt_cmake_export.h)

if((TESTS AND NOT XLNT_SKIP_INTERNAL_TESTS) OR BENCHMARKS)
    target_compile_definitions(xlnt PRIVATE XLNT_EXPOSE_INTERNAL_API=1)
endif()

# requires cmake 3.8+
#target_compile_features(xlnt PUBLIC cxx_std_${XLNT_CXX_LANG})
#target_compile_features(xlnt PUBLIC c_std_${XLNT_C_LANG})

# Includes
target_include_directories(xlnt
	PUBLIC
	$<BUILD_INTERFACE:${XLNT_INCLUDE_DIR}>
	$<INSTALL_INTERFACE:${XLNT_INC_DEST_DIR}>
	PRIVATE
	${XLNT_SOURCE_DIR}
	${XLNT_SOURCE_DIR}/../third-party/libstudxml
    ${XLNT_SOURCE_DIR}/../third-party/miniz
    ${XLNT_SOURCE_DIR}/../third-party/utfcpp/source
    ${XLNT_SOURCE_DIR}/../third-party/fmt/include
    ${XLNT_SOURCE_DIR}/../third-party/fast_float/include)

# ============================================================================
# Link minizip-ng if configured (configured earlier before add_library)
# ============================================================================

if(XLNT_ZIP_BACKEND STREQUAL "minizipng")
    # Link minizip-ng target correctly: prefer provided aliases, then actual target names
    if(TARGET MINIZIP::minizip)
        target_link_libraries(xlnt PRIVATE MINIZIP::minizip)
        message(STATUS "Linking against MINIZIP::minizip")
    elseif(TARGET minizip-ng)
        target_link_libraries(xlnt PRIVATE minizip-ng)
        message(STATUS "Linking against minizip-ng")
    elseif(TARGET minizip)
        target_link_libraries(xlnt PRIVATE minizip)
        message(STATUS "Linking against minizip")
    else()
        message(FATAL_ERROR "minizip-ng target not found (expected MINIZIP::minizip, minizip-ng, or minizip)")
    endif()

    # Dependency targets pass include paths; the following includes are supplementary
    target_include_directories(xlnt PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/minizip-ng)

    # Windows DLL handling: copy required runtime DLLs into the xlnt output directory
    if(WIN32 AND NOT STATIC)
        # zlib(-ng) runtime: handle in-tree and imported targets
        if(TARGET zlib)
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:zlib>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying zlib runtime to xlnt output directory")
        elseif(TARGET zlib-ng)
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:zlib-ng>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying zlib-ng runtime to xlnt output directory")
        elseif(TARGET ZLIB::ZLIB)
            # Use CMake's built-in ZLIB imported target
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:ZLIB::ZLIB>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying ZLIB::ZLIB runtime to xlnt output directory")
        elseif(TARGET ZLIB-NG::ZLIB-NG)
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:ZLIB-NG::ZLIB-NG>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying ZLIB-NG::ZLIB-NG runtime to xlnt output directory")
        endif()

        # minizip(-ng) runtime (only when built as shared)
        if(TARGET MINIZIP::minizip)
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:MINIZIP::minizip>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying minizip-ng runtime to xlnt output directory")
        elseif(TARGET minizip-ng)
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:minizip-ng>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying minizip-ng runtime to xlnt output directory")
        elseif(TARGET minizip)
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:minizip>"
                    "$<TARGET_FILE_DIR:xlnt>"
                COMMENT "Copying minizip runtime to xlnt output directory")
        endif()

        # For CMake >= 3.21, optionally copy transitive runtime DLLs
        if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.21")
            add_custom_command(TARGET xlnt POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_RUNTIME_DLLS:xlnt>
                    $<TARGET_FILE_DIR:xlnt>
                COMMAND_EXPAND_LISTS
                COMMENT "Copying transitive runtime DLLs of xlnt")
        endif()
    endif()
endif()

# Platform- and file-specific settings, MSVC
if(MSVC)
  target_compile_definitions(xlnt PRIVATE _CRT_SECURE_NO_WARNINGS=1)

  if(NOT STATIC)
    target_compile_definitions(xlnt PRIVATE XLNT_EXPORT=1)
  endif()

  set_target_properties(xlnt PROPERTIES
    COMPILE_FLAGS "/wd\"4251\" /wd\"4275\" /wd\"4068\"")
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
    PROPERTIES
    COMPILE_FLAGS "/wd\"4244\" /wd\"4334\" /wd\"4127\"")
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/cryptography/aes.cpp
    PROPERTIES
    COMPILE_FLAGS "/wd\"4996\"")
else()
  # Platform- and file-specific settings, Clang
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
	  PROPERTIES
	  COMPILE_FLAGS "-Wno-undef")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/zip_builtin.cpp
	  PROPERTIES
      COMPILE_FLAGS "-Wno-undef -Wno-shorten-64-to-32")
  endif()

  # Platform- and file-specific settings, GCC
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
	  PROPERTIES
	  COMPILE_FLAGS "-Wno-strict-aliasing")
  endif()
endif()

# Group files into pseudo-folders in IDEs
source_group(xlnt FILES ${ROOT_HEADERS})
source_group(cell FILES ${CELL_HEADERS} ${CELL_SOURCES})
source_group(detail FILES ${DETAIL_ROOT_HEADERS} ${DETAIL_ROOT_SOURCES})
source_group(detail\\cryptography FILES ${DETAIL_CRYPTOGRAPHY_HEADERS} ${DETAIL_CRYPTOGRAPHY_SOURCES})
source_group(detail\\external FILES ${DETAIL_EXTERNAL_HEADERS})
source_group(detail\\header_footer FILES ${DETAIL_HEADER_FOOTER_HEADERS} ${DETAIL_HEADER_FOOTER_SOURCES})
source_group(detail\\implementations FILES ${DETAIL_IMPLEMENTATIONS_HEADERS} ${DETAIL_IMPLEMENTATIONS_SOURCES})
source_group(detail\\number_format FILES ${DETAIL_NUMBER_FORMAT_HEADERS} ${DETAIL_NUMBER_FORMAT_SOURCES})
source_group(detail\\serialization FILES ${DETAIL_SERIALIZATION_HEADERS} ${DETAIL_SERIALIZATION_SOURCES})
source_group(detail\\utils FILES ${DETAIL_UTILS_HEADERS} ${DETAIL_UTILS_SOURCES})
source_group(drawing FILES ${DRAWING_HEADERS} ${DRAWING_SOURCES})
source_group(packaging FILES ${PACKAGING_HEADERS} ${PACKAGING_SOURCES})
source_group(styles FILES ${STYLES_HEADERS} ${STYLES_SOURCES})
source_group(utils FILES ${UTILS_HEADERS} ${UTILS_SOURCES})
source_group(workbook FILES ${WORKBOOK_HEADERS} ${WORKBOOK_SOURCES})
source_group(worksheet FILES ${WORKSHEET_HEADERS} ${WORKSHEET_SOURCES})
source_group(third-party\\miniz FILES ${MINIZ_HEADERS} ${MINIZ_SOURCES})

# Install library
install(TARGETS xlnt EXPORT XlntTargets
  LIBRARY DESTINATION ${XLNT_LIB_DEST_DIR}
  ARCHIVE DESTINATION ${XLNT_LIB_DEST_DIR}
  RUNTIME DESTINATION ${XLNT_BIN_DEST_DIR})

install(EXPORT XlntTargets
  FILE XlntTargets.cmake
  NAMESPACE xlnt::
  DESTINATION ${XLNT_CMAKE_CFG_DEST_DIR})

include(CMakePackageConfigHelpers)

set(XLNT_INCLUDE_INSTALL_DIR ${XLNT_INC_DEST_DIR})
if(CMAKE_INSTALL_PREFIX)
  set(XLNT_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${XLNT_INCLUDE_INSTALL_DIR})
endif()

#See https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html
configure_package_config_file(../cmake/XlntConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/XlntConfig.cmake
  INSTALL_DESTINATION ${XLNT_CMAKE_CFG_DEST_DIR}
  PATH_VARS XLNT_INCLUDE_DIR)

write_basic_package_version_file(XlntConfigVersion.cmake
  COMPATIBILITY ExactVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/XlntConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/XlntConfigVersion.cmake
  DESTINATION ${XLNT_CMAKE_CFG_DEST_DIR})

# Install include directory
install(DIRECTORY ${XLNT_INCLUDE_DIR}/xlnt
  DESTINATION ${XLNT_INC_DEST_DIR}
  PATTERN ".DS_Store" EXCLUDE)

# Install LICENSE.md
install(FILES ${XLNT_ROOT_DIR}/docs/xlnt.3
DESTINATION ${XLNT_MAN_DEST_DIR}/man3)

if(NOT TARGET uninstall)
  # Configure uninstall
  configure_file("${XLNT_ROOT_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  # Add uninstall target
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P
      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

if(NOT MSVC)
  # Set pkg-config variables
  set(PKG_CONFIG_LIBDIR ${XLNT_LIB_DEST_DIR})
  set(PKG_CONFIG_INCLUDEDIR ${XLNT_INC_DEST_DIR})
  set(PKG_CONFIG_LIBS "-L\${libdir} -lxlnt")
  set(PKG_CONFIG_CFLAGS "-I\${includedir}")

  # Replace variables in pkg-config template
  configure_file("${XLNT_ROOT_DIR}/cmake/pkg-config.pc.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xlnt.pc")

  # pkg-config install
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/xlnt.pc"
    DESTINATION ${XLNT_LIB_DEST_DIR}/pkgconfig)
endif()

if(RELEASE)
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_PACKAGE_NAME "xlnt")
  set(CPACK_SUFFIX "${CMAKE_SYSTEM_NAME}")
  if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    string(CONCAT CPACK_SUFFIX ${CPACK_SUFFIX} "-x86")
  else()
    string(CONCAT CPACK_SUFFIX ${CPACK_SUFFIX} "-x64")
  endif()
  if(STATIC)
    string(CONCAT CPACK_SUFFIX ${CPACK_SUFFIX} "-static")
  endif()
  string(TOLOWER "${CPACK_SUFFIX}" CPACK_SUFFIX)
  set(CPACK_SYSTEM_NAME "${CPACK_SUFFIX}")
  include(CPack)
endif()
