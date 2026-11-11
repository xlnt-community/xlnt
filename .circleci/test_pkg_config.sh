#!/usr/bin/env bash
set -euo pipefail

source_dir=$(cd "${1:-$(dirname "$0")/..}" && pwd)
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT

assert_equal()
{
    if [ "$1" != "$2" ]; then
        printf 'Expected: %s\nActual:   %s\n' "$1" "$2" >&2
        exit 1
    fi
}

check_case()
{
    local name=$1 expected_include=$2 expected_lib=$3
    shift 3
    local build_dir="$test_dir/$name"
    mkdir -p "$build_dir"
    (cd "$build_dir" && cmake "$source_dir" -DTESTS=OFF "$@")

    local prefix
    prefix=$(sed -n 's/^CMAKE_INSTALL_PREFIX:PATH=//p' "$build_dir/CMakeCache.txt")
    if [ "$expected_include" = default ]; then
        expected_include="\${prefix}/$(sed -n 's/^CMAKE_INSTALL_INCLUDEDIR:PATH=//p' "$build_dir/CMakeCache.txt")"
    fi
    if [ "$expected_lib" = default ]; then
        expected_lib="\${prefix}/$(sed -n 's/^CMAKE_INSTALL_LIBDIR:PATH=//p' "$build_dir/CMakeCache.txt")"
    fi

    local pc_file="$build_dir/source/xlnt.pc"
    grep -Fx "prefix=$prefix" "$pc_file"
    grep -Fx "includedir=$expected_include" "$pc_file"
    grep -Fx "libdir=$expected_lib" "$pc_file"
    grep -Fx 'exec_prefix=${prefix}' "$pc_file"

    export PKG_CONFIG_PATH= PKG_CONFIG_LIBDIR="$build_dir/source"
    export PKG_CONFIG_ALLOW_SYSTEM_CFLAGS=1 PKG_CONFIG_ALLOW_SYSTEM_LIBS=1
    local effective_prefix include_dir lib_dir
    for effective_prefix in "$prefix" /opt/xlnt-relocated; do
        include_dir=${expected_include//'${prefix}'/$effective_prefix}
        lib_dir=${expected_lib//'${prefix}'/$effective_prefix}
        local pkg_args=(--define-variable="prefix=$effective_prefix")
        assert_equal "$effective_prefix" "$(pkg-config "${pkg_args[@]}" --variable=exec_prefix xlnt)"
        assert_equal "$include_dir" "$(pkg-config "${pkg_args[@]}" --variable=includedir xlnt)"
        assert_equal "$lib_dir" "$(pkg-config "${pkg_args[@]}" --variable=libdir xlnt)"
        assert_equal "-I$include_dir" "$(pkg-config "${pkg_args[@]}" --cflags-only-I xlnt | xargs)"
        assert_equal "-L$lib_dir -lxlnt" "$(pkg-config "${pkg_args[@]}" --libs xlnt | xargs)"
    done
    printf 'PASS: %s\n' "$name"
}

check_case default default default
check_case custom-prefix default default -DCMAKE_INSTALL_PREFIX=/opt/xlnt
check_case relative '${prefix}/headers/xlnt' '${prefix}/lib64' \
    -DCMAKE_INSTALL_PREFIX=/opt/xlnt \
    -DCMAKE_INSTALL_INCLUDEDIR=headers/xlnt -DCMAKE_INSTALL_LIBDIR=lib64
check_case absolute /opt/xlnt-headers /opt/xlnt-libs \
    -DCMAKE_INSTALL_INCLUDEDIR=/opt/xlnt-headers -DCMAKE_INSTALL_LIBDIR=/opt/xlnt-libs
check_case absolute-include /opt/xlnt-headers '${prefix}/lib64' \
    -DCMAKE_INSTALL_INCLUDEDIR=/opt/xlnt-headers -DCMAKE_INSTALL_LIBDIR=lib64
check_case absolute-lib '${prefix}/headers/xlnt' /opt/xlnt-libs \
    -DCMAKE_INSTALL_INCLUDEDIR=headers/xlnt -DCMAKE_INSTALL_LIBDIR=/opt/xlnt-libs
check_case relative-overrides '${prefix}/custom/include' '${prefix}/custom/lib' \
    -DXLNT_INC_DEST_DIR:PATH=custom/include -DXLNT_LIB_DEST_DIR:PATH=custom/lib
check_case absolute-overrides /opt/custom/include /opt/custom/lib \
    -DXLNT_INC_DEST_DIR:PATH=/opt/custom/include -DXLNT_LIB_DEST_DIR:PATH=/opt/custom/lib
