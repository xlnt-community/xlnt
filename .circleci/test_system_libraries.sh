#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 || ( $1 != ON && $1 != OFF ) ]]; then
    echo "Usage: $0 ON|OFF" >&2
    exit 2
fi

scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT
mkdir "$scratch/source" "$scratch/consumer"
git archive HEAD | tar --exclude=third-party -x -C "$scratch/source"
test ! -e "$scratch/source/third-party"

cmake -S "$scratch/source" -B "$scratch/build" \
    -DSTATIC="$1" -DTESTS=ON \
    -DXLNT_USE_SYSTEM_LIBS=ON -DXLNT_USE_OPENSSL=ON \
    -DCMAKE_INSTALL_PREFIX="$scratch/install" -DCMAKE_INSTALL_LIBDIR=lib \
    -DCMAKE_BUILD_TYPE=Release
cmake --build "$scratch/build" --parallel 2
ctest --test-dir "$scratch/build" --output-on-failure
cmake --install "$scratch/build"

cat > "$scratch/consumer/CMakeLists.txt" <<'EOF'
cmake_minimum_required(VERSION 3.13)
project(xlnt_consumer LANGUAGES CXX)
find_package(Xlnt CONFIG REQUIRED)
add_executable(xlnt_consumer main.cpp)
target_link_libraries(xlnt_consumer PRIVATE xlnt::xlnt)
EOF
cat > "$scratch/consumer/main.cpp" <<'EOF'
#include <xlnt/xlnt.hpp>

int main()
{
    xlnt::workbook workbook;
    return workbook.active_sheet().title().empty();
}
EOF
cmake -S "$scratch/consumer" -B "$scratch/consumer-build" \
    -DXlnt_DIR="$scratch/install/lib/cmake/xlnt"
cmake --build "$scratch/consumer-build"
"$scratch/consumer-build/xlnt_consumer"

export PKG_CONFIG_PATH="$scratch/install/lib/pkgconfig${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
export LD_LIBRARY_PATH="$scratch/install/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
pkg_config_args=(--cflags --libs xlnt)
if [[ $1 == ON ]]; then
    pkg_config_args+=(--static)
fi
pkg_config_output=$(pkg-config "${pkg_config_args[@]}")
read -r -a pkg_config_flags <<< "$pkg_config_output"
"${CXX:-c++}" "$scratch/consumer/main.cpp" -o "$scratch/pkg-config-consumer" "${pkg_config_flags[@]}"
"$scratch/pkg-config-consumer"
