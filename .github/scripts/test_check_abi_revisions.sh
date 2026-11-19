#!/usr/bin/env bash
set -euo pipefail

check_revisions=$(cd "$(dirname "$0")" && pwd)/check_abi_revisions.sh
scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT
cd "$scratch"

git init -q -b master
git config user.name 'ABI Test'
git config user.email 'abi-test@example.invalid'
mkdir source
cat > CMakeLists.txt <<'EOF'
cmake_minimum_required(VERSION 3.10)
project(xlnt LANGUAGES C)
add_subdirectory(source)
EOF
cat > source/CMakeLists.txt <<'EOF'
include(abi-version.cmake)
add_library(xlnt SHARED probe.c)
set_target_properties(xlnt PROPERTIES SOVERSION ${abi_version})
EOF
printf '%s\n' 'set(abi_version 1.6.1)' > source/abi-version.cmake
printf '%s\n' 'int kept(void) { return 0; }' 'int exported(void) { return 0; }' > source/probe.c
git add CMakeLists.txt source/
git commit -qm Release
git tag v1.6.1

git switch -qc fork-master
printf '%s\n' 'int kept(void) { return 0; }' > source/probe.c
git commit -qam 'Remove an export in a fork'
git switch -q master
printf '%s\n' 'set(abi_version 2)' > source/abi-version.cmake
git commit -qam 'Introduce SONAME 2'
previous=$(git rev-parse HEAD)

expect_abi_break()
{
    if bash "$check_revisions" "$1" "$2" > report.log 2> build.log ||
        ! grep -F "Comparing with $1" report.log ||
        ! grep -F 'increase SOVERSION' report.log; then
        cat build.log report.log >&2
        return 1
    fi
}

git switch --detach -q "$previous"
git merge -q --no-ff -m 'Synthetic PR merge' fork-master
expect_abi_break "$previous" pull_request

git switch -q master
git cherry-pick fork-master >/dev/null
git commit --allow-empty -qm 'Unrelated follow-up commit in the same push'
expect_abi_break "$previous" push
