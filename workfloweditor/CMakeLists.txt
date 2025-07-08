cmake_minimum_required(VERSION 3.28)
project(WorkflowEditor)
set(BINARY_NAME workfloweditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20 -g -O0 -Wall -Wextra -pedantic -fexperimental-library -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

include(FetchContent)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.28.5  # or latest stable tag
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(IMGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8)
FetchContent_MakeAvailable(IMGUI)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ImGuiFileDialog.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/debug/_deps/imgui-src")

FetchContent_Declare(imnodes
    GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
    GIT_TAG v0.5)
FetchContent_MakeAvailable(imnodes)

add_library(imnodes STATIC
    ${imnodes_SOURCE_DIR}/imnodes.cpp
)

target_include_directories(imnodes PUBLIC ${imnodes_SOURCE_DIR})

# fix: Define IMGUI_DEFINE_MATH_OPERATORS for imnodes
target_compile_definitions(imnodes PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

include_directories(BEFORE "${CMAKE_SOURCE_DIR}/edencore")
include_directories(BEFORE "${CMAKE_SOURCE_DIR}/edenanalytics")
include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")
include_directories(BEFORE "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")

set(SOURCES 
    main.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)   

add_executable(${BINARY_NAME} ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building in Debug mode")
    include_directories(BEFORE "../debug/_deps/imgui-src/include")
    include_directories(BEFORE "../debug/_deps/imgui-src")
    include_directories(BEFORE "../debug/_deps/imgui-src/backends")
    include_directories(BEFORE "../debug/_deps/imnodes-src")
    include_directories(BEFORE "../debug/_deps/sdl2-src/include")
    include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")
    include_directories(BEFORE "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")
    target_compile_definitions(${BINARY_NAME} PRIVATE DEBUG_MODE)
    target_compile_options(${BINARY_NAME} PRIVATE -g -O0 -Wreturn-stack-address -I/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building in Release mode")
    include_directories(BEFORE "../release/_deps/imgui-src/include")
    include_directories(BEFORE "../release/_deps/imgui-src")
    include_directories(BEFORE "../release/_deps/imgui-src/backends")
    include_directories(BEFORE "../release/_deps/imnodes-src")
    include_directories(BEFORE "../release/_deps/sdl2-src/include")
    include_directories(BEFORE "/opt/homebrew/Cellar/nlohmann-json/3.11.3/include")
    include_directories(BEFORE "/opt/homebrew/Cellar/llvm/19.1.7/include/c++/v1")
    target_compile_options(${BINARY_NAME} PRIVATE -O3)  # Example flags for GCC/Clang
endif()

find_package(OpenGL REQUIRED)
# find_package(SDL2 REQUIRED)  # it is included in the CMakeLists.txt

# target_link_libraries(${BINARY_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${BINARY_NAME} PRIVATE edencore edenanalytics SDL2::SDL2 OpenGL::GL imgui imnodes)
