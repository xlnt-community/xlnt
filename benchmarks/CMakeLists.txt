cmake_minimum_required(VERSION 3.1...3.31)
project(xlnt.benchmarks)

set(CMAKE_CXX_STANDARD ${XLNT_CXX_LANG})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD ${XLNT_C_LANG})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

if(NOT COMBINED_PROJECT)
  # Include xlnt library
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../source ${CMAKE_CURRENT_BINARY_DIR}/source)
endif()

if(STATIC_CRT)
  include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ucm.cmake)
  ucm_set_runtime(STATIC)
endif()

set(XLNT_BENCHMARK_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

file(GLOB BENCHMARK_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(COVERAGE AND STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

foreach(BENCHMARK_SOURCE IN ITEMS ${BENCHMARK_SOURCES})
  # Convert <name>.cpp to benchmark-<name>
  get_filename_component(BENCHMARK_NAME ${BENCHMARK_SOURCE} NAME_WE)
  set(BENCHMARK_EXECUTABLE benchmark-${BENCHMARK_NAME})

  add_executable(${BENCHMARK_EXECUTABLE} ${BENCHMARK_SOURCE})

  target_link_libraries(${BENCHMARK_EXECUTABLE} PRIVATE xlnt)
  # Need to use some test helpers
  target_include_directories(${BENCHMARK_EXECUTABLE}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../tests)
  # Some helpers also need further internal includes
  target_include_directories(${BENCHMARK_EXECUTABLE}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../source)
  target_compile_definitions(${BENCHMARK_EXECUTABLE}
    PRIVATE XLNT_BENCHMARK_DATA_DIR="${XLNT_BENCHMARK_DATA_DIR}")

  if (XLNT_USE_LOCALE_COMMA_DECIMAL_SEPARATOR)
    target_compile_definitions(${BENCHMARK_EXECUTABLE} PRIVATE XLNT_USE_LOCALE_COMMA_DECIMAL_SEPARATOR=1)
  endif()
  target_compile_definitions(${BENCHMARK_EXECUTABLE} PRIVATE XLNT_LOCALE_COMMA_DECIMAL_SEPARATOR="${XLNT_LOCALE_COMMA_DECIMAL_SEPARATOR}")

  if (XLNT_USE_LOCALE_ARABIC_DECIMAL_SEPARATOR)
    target_compile_definitions(${BENCHMARK_EXECUTABLE} PRIVATE XLNT_USE_LOCALE_ARABIC_DECIMAL_SEPARATOR=1)
  endif()
  target_compile_definitions(${BENCHMARK_EXECUTABLE} PRIVATE XLNT_LOCALE_ARABIC_DECIMAL_SEPARATOR="${XLNT_LOCALE_ARABIC_DECIMAL_SEPARATOR}")


  if(MSVC AND NOT STATIC)
    # Copy xlnt DLL into benchmarks directory
    add_custom_command(TARGET ${BENCHMARK_EXECUTABLE} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:xlnt>
      $<TARGET_FILE_DIR:${BENCHMARK_EXECUTABLE}>)
  endif()
endforeach()

option(XLNT_MICROBENCH_ENABLED "Enable small benchmarks typically used for development" OFF)
if (XLNT_MICROBENCH_ENABLED)
	add_subdirectory(microbenchmarks)
endif()
