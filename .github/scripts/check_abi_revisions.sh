#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 || ! $1 =~ ^[0-9a-f]{40}$ || ( $2 != pull_request && $2 != push ) ]]; then
    echo "Usage: $0 BASELINE_SHA pull_request|push" >&2
    exit 2
fi

baseline=$1
event=$2
case "$event" in
    pull_request)
        if [[ $(git rev-parse HEAD^1) != "$baseline" ]] || ! git rev-parse --verify -q HEAD^2 >/dev/null; then
            echo 'Expected the PR merge commit with the event base as its first parent.' >&2
            exit 1
        fi
        ;;
    push)
        if [[ $baseline == 0000000000000000000000000000000000000000 ]]; then
            baseline=$(git describe --first-parent --tags --match 'v[0-9]*' --abbrev=0 HEAD^)
        elif ! git cat-file -e "$baseline^{commit}" 2>/dev/null; then
            git fetch --no-tags origin "$baseline"
        fi
        ;;
esac

baseline=$(git rev-parse --verify "$baseline^{commit}")
baselines=("$baseline")
if [[ $event == push ]] &&
    release=$(git describe --first-parent --tags --match 'v[0-9]*' --abbrev=0 "$baseline") &&
    [[ $(git rev-parse "$release^{commit}") != "$baseline" ]]; then
    baselines+=("$release")
fi

check_abi=$(cd "$(dirname "$0")" && pwd)/check_abi.sh
scratch=$(mktemp -d)
trap 'git worktree remove --force "$scratch/baseline" 2>/dev/null || true; rm -rf "$scratch"' EXIT

build_shared_library()
{
    git -C "$1" submodule update --init --recursive >&2
    cmake -S "$1" -B "$2" -DSTATIC=OFF -DTESTS=OFF -DCMAKE_BUILD_TYPE=RelWithDebInfo >&2
    cmake --build "$2" --target xlnt --parallel 2 >&2
}

build_shared_library "$PWD" "$scratch/candidate-build"
for revision in "${baselines[@]}"; do
    git worktree add --detach "$scratch/baseline" "$revision" >&2
    build_shared_library "$scratch/baseline" "$scratch/baseline-build"
    printf '\nComparing with %s\n' "$revision"
    bash "$check_abi" "$scratch/baseline-build/source/libxlnt.so" \
        "$scratch/candidate-build/source/libxlnt.so" 2>&1
    git worktree remove --force "$scratch/baseline"
    rm -rf "$scratch/baseline-build"
done
