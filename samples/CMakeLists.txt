cmake_minimum_required(VERSION 3.1...3.31)
project(xlnt.samples)

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

set(XLNT_SAMPLE_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

file(GLOB SAMPLE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(COVERAGE AND STATIC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

foreach(SAMPLE_SOURCE IN ITEMS ${SAMPLE_SOURCES})
  # Convert <name>.cpp to sample-<name>
  get_filename_component(SAMPLE_NAME ${SAMPLE_SOURCE} NAME_WE)
  set(SAMPLE_EXECUTABLE sample-${SAMPLE_NAME})

  add_executable(${SAMPLE_EXECUTABLE} ${SAMPLE_SOURCE})

  target_link_libraries(${SAMPLE_EXECUTABLE} PRIVATE xlnt)
  # Need to use some test helpers
  target_include_directories(${SAMPLE_EXECUTABLE}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../tests)
  # Some helpers also need further internal includes
  target_include_directories(${SAMPLE_EXECUTABLE}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../source)
  target_compile_definitions(${SAMPLE_EXECUTABLE}
    PRIVATE XLNT_SAMPLE_DATA_DIR="${XLNT_SAMPLE_DATA_DIR}")

  if(MSVC AND NOT STATIC)
    # Copy xlnt DLL into samples directory
    add_custom_command(TARGET ${SAMPLE_EXECUTABLE} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:xlnt>
      $<TARGET_FILE_DIR:${SAMPLE_EXECUTABLE}>)
  endif()
endforeach()
#
