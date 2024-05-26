eden project:

edenbin: 
    binary
    test features

edencore:
    library
    core classes and functions
        thread safe singleton
        logger manager
        datetime


edentests:
    binary using GoogleTest
    unit testing

outputs:
    build/Debug/bin
    build/Debug/lib

# C/C++ configuration in VS Code:
includePath
${default}
${workspaceFolder}/edencore
/opt/homebrew/Cellar/fmt/10.1.1/include
${workspaceFolder}/build/_deps/googletest-src/googletest/include/gtest

C++ Standard
C++20