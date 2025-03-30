
#include <gtest/gtest.h>
#include "sort_iterative.h"
#include "randomnumbers.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

/**
 * @brief QuickSortIterTest class
 * @details Test fixture for Quick Sort Iterative
 * 
 */
class QuickSortIterTest : public ::testing::Test {
protected:
    std::vector<int> randomNumbers {};
    std::vector<float> randomFloatNumbers {};
    
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

        float fmin = 1.0;
        float fmax = 1000.0;
        randomFloatNumbers = generateRandomNumbers<float>(num, fmin, fmax);
    }
};
    
// Test sorting an unsorted vector
TEST_F(QuickSortIterTest, SortUnsortedVector) {
    std::vector<int> vec = {5, 3, 8, 1, 2};
    eden::sort_iter::quickSort<int>(vec);
    
    // Check that the vector is now sorted
    EXPECT_TRUE(isSorted<int>(vec));
}

// Test sorting an already sorted vector
TEST_F(QuickSortIterTest, SortSortedVector) {
    std::vector<int> vec = {1, 2, 3, 5, 8};
    eden::sort_iter::quickSort<int>(vec);
    
    // Check that the vector is still sorted
    EXPECT_TRUE(isSorted<int>(vec));
}
    
// Test sorting a vector with one element
TEST_F(QuickSortIterTest, SortSingleElement) {
    std::vector<int> vec = {42};
    eden::sort_iter::quickSort<int>(vec);
    
    // A single element is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting an empty vector
TEST_F(QuickSortIterTest, SortEmptyVector) {
    std::vector<int> vec = {};
    eden::sort_iter::quickSort<int>(vec);
    
    // An empty vector is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting a vector with duplicate elements
TEST_F(QuickSortIterTest, SortVectorWithDuplicates) {
    std::vector<int> vec = {4, 2, 5, 5, 1, 2, 3};
    eden::sort_iter::quickSort<int>(vec);
    
    // Check that the vector is sorted correctly, including duplicates
    EXPECT_TRUE(isSorted(vec));
}
    
TEST_F(QuickSortIterTest, SortRandomNumbers) {
    eden::sort_iter::quickSort<int>(randomNumbers);
    
    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomNumbers));
}

TEST_F(QuickSortIterTest, SortsFloatingPointNumbers) {
    eden::sort_iter::quickSort<float>(randomFloatNumbers);
    
    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomFloatNumbers));
}

TEST_F(QuickSortIterTest, SortsStrings) {
    std::vector<std::string> arr = {"banana", "apple", "cherry", "date"};
    eden::sort_iter::quickSort<std::string>(arr);
    EXPECT_TRUE(isSorted(arr));
}
