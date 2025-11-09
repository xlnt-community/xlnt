cmake_minimum_required(VERSION 3.1...3.31)
project(xlnt.test)

set(CMAKE_CXX_STANDARD ${XLNT_CXX_LANG})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD ${XLNT_C_LANG})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

# Options
option(XLNT_SKIP_INTERNAL_TESTS "Skip internal tests. Internal API will not be exported. Especially useful for CI testing." OFF)
mark_as_advanced(XLNT_SKIP_INTERNAL_TESTS)

if(NOT COMBINED_PROJECT)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../source ${CMAKE_CURRENT_BINARY_DIR}/source)
endif()

if(STATIC_CRT)
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ucm.cmake)
  ucm_set_runtime(STATIC)
endif()

file(GLOB CELL_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/cell/*.cpp)
file(GLOB DRAWING_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB DETAIL_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/detail/*.cpp)
file(GLOB INTERNAL_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/internal/*.cpp)
file(GLOB PACKAGING_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/packaging/*.cpp)
file(GLOB STYLES_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/styles/*.cpp)
file(GLOB UTILS_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp)
file(GLOB WORKBOOK_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/workbook/*.cpp)
file(GLOB WORKSHEET_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/worksheet/*.cpp)

set(TESTS
  ${CELL_TESTS}
  ${DRAWING_TESTS}
  ${DETAIL_TESTS}
  ${PACKAGING_TESTS}
  ${STYLES_TESTS}
  ${UTILS_TESTS}
  ${WORKBOOK_TESTS}
  ${WORKSHEET_TESTS})

if (NOT XLNT_SKIP_INTERNAL_TESTS)
  list(APPEND TESTS ${INTERNAL_TESTS})
endif()

file(GLOB HELPERS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/helpers/*.hpp)
file(GLOB HELPERS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/helpers/*.cpp)

file(GLOB HELPERS_INTERNAL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/helpers/internal/*.hpp)

set(HELPERS ${HELPERS_HEADERS} ${HELPERS_SOURCES})
set(HELPERS_INTERNAL ${HELPERS_INTERNAL_HEADERS})
set(RUNNER ${CMAKE_CURRENT_SOURCE_DIR}/runner.cpp)

if(COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_executable(xlnt.test ${RUNNER} ${TESTS} ${HELPERS} ${HELPERS_INTERNAL} $<TARGET_OBJECTS:libstudxml>)
target_link_libraries(xlnt.test PRIVATE xlnt)
target_include_directories(xlnt.test
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if (NOT XLNT_SKIP_INTERNAL_TESTS)
  target_include_directories(xlnt.test
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../source
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/libstudxml
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/miniz
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/fast_float/include)
endif()

set(XLNT_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
target_compile_definitions(xlnt.test PRIVATE XLNT_TEST_DATA_DIR="${XLNT_TEST_DATA_DIR}")
# requires cmake 3.8+
#target_compile_features(xlnt.test PRIVATE cxx_std_${XLNT_CXX_LANG})
#target_compile_features(xlnt.test PRIVATE c_std_${XLNT_C_LANG})

if (XLNT_USE_LOCALE_COMMA_DECIMAL_SEPARATOR)
  target_compile_definitions(xlnt.test PRIVATE XLNT_USE_LOCALE_COMMA_DECIMAL_SEPARATOR=1)
endif()
target_compile_definitions(xlnt.test PRIVATE XLNT_LOCALE_COMMA_DECIMAL_SEPARATOR="${XLNT_LOCALE_COMMA_DECIMAL_SEPARATOR}")

if (XLNT_USE_LOCALE_ARABIC_DECIMAL_SEPARATOR)
  target_compile_definitions(xlnt.test PRIVATE XLNT_USE_LOCALE_ARABIC_DECIMAL_SEPARATOR=1)
endif()
target_compile_definitions(xlnt.test PRIVATE XLNT_LOCALE_ARABIC_DECIMAL_SEPARATOR="${XLNT_LOCALE_ARABIC_DECIMAL_SEPARATOR}")

if(MSVC)
  # bigobj because there are so many headers in one source file
  set_target_properties(xlnt.test PROPERTIES COMPILE_FLAGS "/wd\"4068\" /bigobj")
endif()

source_group(helpers FILES ${HELPERS})
source_group(helpers\\internal FILES ${HELPERS_INTERNAl})
source_group(runner FILES ${RUNNER})
source_group(tests\\cell FILES ${CELL_TESTS})
source_group(tests\\detail FILES ${DETAIL_TESTS})
source_group(tests\\internal FILES ${INTERNAL_TESTS})
source_group(tests\\drawing FILES ${DRAWING_TESTS})
source_group(tests\\packaging FILES ${PACKAGING_TESTS})
source_group(tests\\serialization FILES ${SERIALIZATION_TESTS})
source_group(tests\\styles FILES ${STYLES_TESTS})
source_group(tests\\utils FILES ${UTILS_TESTS})
source_group(tests\\workbook FILES ${WORKBOOK_TESTS})
source_group(tests\\worksheet FILES ${WORKSHEET_TESTS})

if(MSVC AND NOT STATIC)
  # copy xlnt DLL into xlnt.test directory
  add_custom_command(TARGET xlnt.test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:xlnt>
    $<TARGET_FILE_DIR:xlnt.test>)
endif()

# Use add_test() for CTest support
add_test(NAME xlnt_test COMMAND xlnt.test)
