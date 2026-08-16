#!/usr/bin/env bash
# Build rodd for aarch64 Android using the NDK.
set -e
NDK="${NDK:-$HOME/Android/Sdk/ndk/27.0.12077973}"
CC="$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang"
OUT="${1:-webroot/rodd}"
"$CC" -O2 -Wall -Wextra -Wno-unused-parameter -o "$OUT" rodd.c
"$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" "$OUT"
echo "built: $OUT"
