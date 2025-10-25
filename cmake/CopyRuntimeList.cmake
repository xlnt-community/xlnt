if(NOT DEFINED files)
  return()
endif()

if(NOT DEFINED dest)
  message(FATAL_ERROR "CopyRuntimeList.cmake: 'dest' is not defined")
endif()

file(MAKE_DIRECTORY "${dest}")

foreach(f IN LISTS files)
  if(EXISTS "${f}")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${f}" "${dest}")
  endif()
endforeach()

