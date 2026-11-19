#!/usr/bin/env bash
set -euo pipefail

check_abi=$(cd "$(dirname "$0")" && pwd)/check_abi.sh
scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT
cd "$scratch"

printf '%s\n' 'int kept(void) { return 0; }' > broken.c
cp broken.c baseline.c
printf '%s\n' 'int removed(void) { return 0; }' >> baseline.c
cp baseline.c compatible.c
printf '%s\n' 'int added(void) { return 0; }' >> compatible.c

build_library()
{
    cc -fPIC -shared "-Wl,-soname,libxlnt.so.$1" "$2.c" -o "$3.so" "${@:4}"
}

expect_failure()
{
    if bash "$check_abi" "$1" "$2" > result.log 2>&1; then
        cat result.log
        printf 'Expected the comparison of %s and %s to fail.\n' "$1" "$2" >&2
        return 1
    fi
    grep -F "$3" result.log || { cat result.log; return 1; }
}

build_library 2 baseline baseline -g
build_library 2 compatible compatible -g
build_library 2 broken broken -g
build_library 3 broken bumped -g
build_library 1 broken lowered -g
build_library 1.6.1 baseline legacy -g
build_library 2 baseline no-debug

bash "$check_abi" baseline.so baseline.so
bash "$check_abi" baseline.so compatible.so
expect_failure baseline.so broken.so 'increase SOVERSION'
bash "$check_abi" baseline.so bumped.so
expect_failure baseline.so lowered.so 'SOVERSION decreased'
bash "$check_abi" legacy.so baseline.so
expect_failure baseline.so no-debug.so 'Missing DWARF debug information'

mkdir legacy new
cp legacy.so legacy/libxlnt.so.1.6.1
cp baseline.so new/libxlnt.so.1.6.1
expect_failure legacy/libxlnt.so.1.6.1 new/libxlnt.so.1.6.1 'filename conflicts'
mv new/libxlnt.so.1.6.1 new/libxlnt.so.2.1.6.1
bash "$check_abi" legacy/libxlnt.so.1.6.1 new/libxlnt.so.2.1.6.1
