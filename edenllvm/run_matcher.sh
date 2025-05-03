#!/bin/bash
# This script runs the string matcher tool on a given C++ file.
# It sets up the necessary environment variables and paths for the LLVM toolchain.
# Usage: ./run_matcher.sh <path_to_cpp_file>

# Paths
LLVM_ROOT="/opt/homebrew/Cellar/llvm/19.1.7"
SDKROOT=$(xcrun --show-sdk-path)
RESOURCE_DIR="$LLVM_ROOT/lib/clang/19"
CXX_STD_INCLUDE="$LLVM_ROOT/include/c++/v1"

echo "📁 SDK Path:         $SDKROOT"
echo "📁 Clang Resource:   $RESOURCE_DIR"
echo "📁 libc++ includes:  $CXX_STD_INCLUDE"
echo "🧪 Analyzing file:   $1"
echo "🛠  Command:"
echo ".debug/Debug/bin/string_matcher $1 -- -std=c++20 \\"
echo "  -isysroot \"$SDKROOT\" \\"
echo "  -resource-dir \"$RESOURCE_DIR\" \\"
echo "  -I\"$CXX_STD_INCLUDE\" \\"
echo "  -stdlib=libc++"
echo

# Run the matcher tool
./debug/Debug/bin/string_matcher "$1" -- \
  -std=c++20 \
  -isysroot "$SDKROOT" \
  -resource-dir "$RESOURCE_DIR" \
  -I"$CXX_STD_INCLUDE" \
  -stdlib=libc++