
#include <gtest/gtest.h>
#include "sort.h"
#include "randomnumbers.h"
#include "time_elapsed.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

/**
 * @brief BubbleSortTest class
 * @details Test fixture for Bubble Sort
 * 
 */
class BubbleSortTest : public ::testing::Test {
protected:
    std::vector<int> randomNumbers {};

    // Helper function to compare two vectors for equality
    template <typename T>
    bool isSorted(const std::vector<T>& vec) {
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i - 1] > vec[i]) return false;
        }
        return true;
    }

public:
    void SetUp() override {
        // Generate num random numbers between min and max
        int num = 10000;
        int min = 1;
        int max = 1000;

        randomNumbers = generateRandomNumbers<int>(num, min, max);
    }
};

// Test sorting an unsorted vector
TEST_F(BubbleSortTest, SortUnsortedVector) {
    std::vector<int> vec = {5, 3, 8, 1, 2};
    eden::bubbleSort<int>(vec);

    // Check that the vector is now sorted
    EXPECT_TRUE(isSorted<int>(vec));
}

// Test sorting an already sorted vector
TEST_F(BubbleSortTest, SortSortedVector) {
    std::vector<int> vec = {1, 2, 3, 5, 8};
    eden::bubbleSort<int>(vec);

    // Check that the vector is still sorted
    EXPECT_TRUE(isSorted<int>(vec));
}

// Test sorting a vector with one element
TEST_F(BubbleSortTest, SortSingleElement) {
    std::vector<int> vec = {42};
    eden::bubbleSort<int>(vec);

    // A single element is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}

// Test sorting an empty vector
TEST_F(BubbleSortTest, SortEmptyVector) {
    std::vector<int> vec = {};
    eden::bubbleSort<int>(vec);

    // An empty vector is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}

// Test sorting a vector with duplicate elements
TEST_F(BubbleSortTest, SortVectorWithDuplicates) {
    std::vector<int> vec = {4, 2, 5, 5, 1, 2, 3};
    eden::bubbleSort<int>(vec);

    // Check that the vector is sorted correctly, including duplicates
    EXPECT_TRUE(isSorted(vec));
}

TEST_F(BubbleSortTest, SortRandomNumbers) {
    eden::sTimeElapsed te;
    te.timeStart();
    eden::bubbleSort<int>(randomNumbers);
    te.nsElapsed();

    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomNumbers));
}