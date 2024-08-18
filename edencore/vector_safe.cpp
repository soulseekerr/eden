
#include "vector_safe.h"
#include <iostream>

// Test Example
int example_vector_safe() {
    VectorSafe_t<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(i);
    }

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }

    std::cout << "\nCapacity: " << vec.capacity() << std::endl;
    return 0;
}