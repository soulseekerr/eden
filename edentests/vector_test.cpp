
#include <gtest/gtest.h>
#include "vector_safe.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

TEST(VectorSafeTest, BasicAssertions) {

    std::cout << "Testing VectorSafe_t" << std::endl;

#ifdef DEBUG_MODE
    std::cout << "Debug mode is enabled!" << std::endl;
    // Add any additional debug-specific code here
#endif

    auto vSize = 100000;

    VectorSafe_t<int> intVec(vSize);

    std::mt19937_64 rng{std::random_device{}()};
    std::uniform_real_distribution<double> dist(1.0, 10000.0);
    for (auto i = 0; i < vSize; ++i) {
        intVec.push_back(dist(rng));
    }

    std::cout << "intVec size = " << intVec.size() << " capacity = " << intVec.capacity() << std::endl;

    EXPECT_EQ(intVec.size(), intVec.capacity());
}