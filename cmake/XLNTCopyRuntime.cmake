#[======================================================================
# xlnt runtime DLL copy helper
#
# Function: xlnt_copy_runtime(<target> [dep_target ...])
# - Windows-only: copy dependent DLLs next to <target> output. No-op for
#   static libs or non-Windows platforms.
# - For each provided CMake target name (e.g. ZLIB::ZLIB, zlib-ng,
#   MINIZIP::minizip), if the target exists, add a POST_BUILD command to copy
#   its runtime to $<TARGET_FILE_DIR:<target>> using generator expressions so
#   multi-config (Debug/Release) and suffixed names resolve correctly.
#
# Examples:
#   xlnt_copy_runtime(xlnt ZLIB::ZLIB zlib-ng zlib MINIZIP::minizip minizip-ng minizip)
#   xlnt_copy_runtime(xlnt.test ZLIB::ZLIB zlib-ng zlib MINIZIP::minizip minizip-ng minizip)
#=======================================================================]

# Record the directory of this helper at include-time so calls from other
# directories (e.g. source/CMakeLists.txt) can still locate auxiliary scripts.
set(XLNT_COPY_RUNTIME_LIST_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/CopyRuntimeList.cmake")

function(xlnt_copy_runtime target)
    if(NOT WIN32)
        return()
    endif()

    get_target_property(_tgt_type ${target} TYPE)
    if(NOT _tgt_type)
        message(FATAL_ERROR "xlnt_copy_runtime: target '${target}' does not exist")
    endif()

    foreach(dep IN LISTS ARGN)
        if(TARGET ${dep})
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${dep}>
                    $<TARGET_FILE_DIR:${target}>/
                COMMENT "Copy runtime: ${dep} -> $<TARGET_FILE_DIR:${target}>")
        endif()
    endforeach()

    # Additionally copy transitive runtime DLLs of <target> (CMake >= 3.21).
    # Use a helper script that tolerates an empty list without failing.
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.21")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
                -Dfiles=$<TARGET_RUNTIME_DLLS:${target}>
                -Ddest=$<TARGET_FILE_DIR:${target}>
                -P ${XLNT_COPY_RUNTIME_LIST_SCRIPT}
            COMMAND_EXPAND_LISTS
            COMMENT "Copy transitive runtime DLLs of ${target}")
    endif()
endfunction()
