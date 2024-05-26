
#include "hello.h"
#include <iostream>
#include <fmt/core.h>

void hello() {
    std::string names = "Vincent and Anna";
    std::cout << "Hello " << names << "! (std::cout)" << std::endl;
    fmt::print("Hello {}! (fmt::print)\n", names);
}