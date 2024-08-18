
#include "vector.h"

#include <iostream>

using namespace eden;

void example_vector() {

    Vector_t<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // vec.insert(1, 10);
    // for (size_t i = 0; i < vec.size(); ++i) {
    //     std::cout << vec[i] << " ";
    // }
    // std::cout << std::endl;

    vec.erase(1);
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}