
#include <gtest/gtest.h>
#include "sort.h"
#include "randomnumbers.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

/**
 * @brief MergeSortTest class
 * @details Test fixture for Merge Sort
 * 
 */
class MergeSortTest : public ::testing::Test {
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
TEST_F(MergeSortTest, SortUnsortedVector) {
    std::vector<int> vec = {5, 3, 8, 1, 2};
    eden::mergeSortFull<int>(vec);
    
    // Check that the vector is now sorted
    EXPECT_TRUE(isSorted<int>(vec));
}
    
// Test sorting an already sorted vector
TEST_F(MergeSortTest, SortSortedVector) {
    std::vector<int> vec = {1, 2, 3, 5, 8};
    eden::mergeSortFull<int>(vec);
    
    // Check that the vector is still sorted
    EXPECT_TRUE(isSorted<int>(vec));
}
    
// Test sorting a vector with one element
TEST_F(MergeSortTest, SortSingleElement) {
    std::vector<int> vec = {42};
    eden::mergeSortFull<int>(vec);
    
    // A single element is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting an empty vector
TEST_F(MergeSortTest, SortEmptyVector) {
    std::vector<int> vec = {};
    eden::mergeSortFull<int>(vec);
    
    // An empty vector is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting a vector with duplicate elements
TEST_F(MergeSortTest, SortVectorWithDuplicates) {
    std::vector<int> vec = {4, 2, 5, 5, 1, 2, 3};
    eden::mergeSortFull<int>(vec);
    
    // Check that the vector is sorted correctly, including duplicates
    EXPECT_TRUE(isSorted(vec));
}
    
TEST_F(MergeSortTest, SortRandomNumbers) {
    eden::mergeSortFull<int>(randomNumbers);
    
    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomNumbers));
}