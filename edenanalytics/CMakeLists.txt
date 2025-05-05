cmake_minimum_required(VERSION 3.28.1)
set(MY_LIB_NAME edenanalytics)
project(${MY_LIB_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -Wall -fexperimental-library -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

include_directories(BEFORE "/opt/homebrew/Cellar/fmt/10.1.1/include")
include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")
include_directories((BEFORE "../edencore"))
include_directories((BEFORE "../edenanalytics"))

set(SOURCES
    workflow/workflow.cpp
    core/yieldcurve.cpp
    core/creditcurve.cpp
    models/hullwhite_1factor.cpp
    edenanalytics.cpp
    task/computepresentvalue.cpp
    task/tasknode.cpp
    task/computetest.cpp
)

add_library(libfmt SHARED IMPORTED)
add_library(${MY_LIB_NAME} SHARED ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    target_compile_definitions(${MY_LIB_NAME} PRIVATE DEBUG_MODE)
    target_compile_options(${MY_LIB_NAME} PRIVATE -g -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    target_compile_options(${MY_LIB_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
endif()

find_package(fmt)
target_link_libraries(${MY_LIB_NAME} PRIVATE fmt::fmt edencore)
target_compile_features(${MY_LIB_NAME} PRIVATE cxx_std_20)