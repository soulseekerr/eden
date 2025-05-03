# CMakeLists.txt for Clang AST Matcher Tool
cmake_minimum_required(VERSION 3.28.1)
project(ClangStringReturnMatcher)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -std=c++20 -g -O0 -Wall -fexperimental-library -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fix for macOS with Homebrew LLVM
execute_process(COMMAND xcrun --show-sdk-path OUTPUT_VARIABLE SDK_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(SYSTEM ${SDK_PATH}/usr/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot ${SDK_PATH}")

add_executable(string_matcher main.cpp)

target_link_libraries(string_matcher
  PRIVATE
  clangTooling
  clangBasic
  clangASTMatchers
)
