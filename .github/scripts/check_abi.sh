#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 BASELINE_LIBRARY CANDIDATE_LIBRARY" >&2
    exit 2
fi

soname()
{
    LC_ALL=C readelf -d "$1" | sed -n 's/.*(SONAME).*\[\([^]]*\)\].*/\1/p'
}

baseline_soname=$(soname "$1")
candidate_soname=$(soname "$2")
printf 'Baseline SONAME: %s\nCandidate SONAME: %s\n' "$baseline_soname" "$candidate_soname"

integer_soname='^libxlnt\.so\.(0|[1-9][0-9]*)$'
if [[ $baseline_soname != libxlnt.so.* || ! $candidate_soname =~ $integer_soname ]]; then
    echo 'Expected versioned xlnt libraries and an integer candidate SOVERSION.' >&2
    exit 1
fi

candidate_version=${BASH_REMATCH[1]}
if [[ $baseline_soname =~ $integer_soname ]] &&
    (( candidate_version < BASH_REMATCH[1] )); then
    echo 'SOVERSION decreased.' >&2
    exit 1
fi

if [[ $baseline_soname != "$candidate_soname" ]]; then
    baseline_file=$(realpath -- "$1")
    candidate_file=$(realpath -- "$2")
    if [[ ${candidate_file##*/} == "${baseline_file##*/}" ||
          ${candidate_file##*/} == "$baseline_soname" ||
          ${baseline_file##*/} == "$candidate_soname" ]]; then
        echo 'Library filename conflicts with the other ABI. Give each ABI a distinct filename.' >&2
        exit 1
    fi
fi

for library in "$@"; do
    if ! LC_ALL=C readelf --section-headers --wide "$library" |
        grep -E '[[:space:]]\.z?debug_info[[:space:]]' >/dev/null; then
        printf 'Missing DWARF debug information in %s.\n' "$library" >&2
        exit 1
    fi
done

abi_changed=4
abi_incompatible=8
status=0
abidiff --ignore-soname --no-added-syms "$1" "$2" || status=$?
if (( status & ~(abi_changed | abi_incompatible) )); then
    printf 'abidiff failed (status %d).\n' "$status" >&2
    exit 1
fi

if (( status & abi_incompatible )) && [[ $baseline_soname == "$candidate_soname" ]]; then
    printf 'Incompatible ABI change with SONAME %s. Restore ABI compatibility or increase SOVERSION.\n' \
        "$candidate_soname" >&2
    exit 1
fi

if (( (status & abi_changed) && !(status & abi_incompatible) )); then
    echo 'Review the reported ABI changes for compatibility.'
fi
