#!/usr/bin/env bash
# Build rodd for aarch64 Android using the NDK.
set -e
NDK="${NDK:-$HOME/Android/Sdk/ndk/27.0.12077973}"
CC="$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang"
OUT="${1:-webroot/rodd}"

echo "[build.sh] NDK path: $NDK"
echo "[build.sh] Compiler: $CC"
echo "[build.sh] Output: $OUT"

if [ ! -f "$CC" ]; then
    echo "[build.sh] ERROR: Compiler not found at $CC"
    echo "[build.sh] Please set NDK=/path/to/ndk or install NDK 27.0.12077973"
    exit 1
fi

echo "[build.sh] Compiling rodd.c..."
"$CC" -O2 -Wall -Wextra -Wno-unused-parameter -o "$OUT" rodd.c
echo "[build.sh] Compilation complete"

echo "[build.sh] Stripping binary..."
"$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip" "$OUT"

if [ -f "$OUT" ]; then
    SIZE=$(stat -f%z "$OUT" 2>/dev/null || stat -c%s "$OUT" 2>/dev/null || echo "?")
    echo "[build.sh] ✓ Built successfully: $OUT (size: $SIZE)"
else
    echo "[build.sh] ERROR: Output file not created"
    exit 1
fi
