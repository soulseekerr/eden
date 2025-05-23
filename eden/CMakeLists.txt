cmake_minimum_required(VERSION 3.28.1)
set(MY_APP_NAME eden)
project(${MY_APP_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -fexperimental-library -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

include_directories((BEFORE "./"))
include_directories((BEFORE "../build/_deps/googletest-src/googletest/include/gtest"))
include_directories((BEFORE "../edencore"))
include_directories((BEFORE "../edenanalytics"))
include_directories(BEFORE "/opt/homebrew/Cellar/fmt/10.1.1/include")
include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")

add_library(libfmt SHARED IMPORTED)

set(SOURCES main.cpp)

add_executable(${MY_APP_NAME} ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${MY_APP_NAME} PRIVATE DEBUG_MODE)
    target_compile_options(${MY_APP_NAME} PRIVATE -g -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${MY_APP_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
endif()

find_package(fmt)
target_link_libraries(${MY_APP_NAME} PRIVATE edencore edenanalytics fmt::fmt)
target_compile_features(${MY_APP_NAME} PRIVATE cxx_std_20)