
#include <gtest/gtest.h>
#include "sort.h"
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
 * @brief QuickSortTest class
 * @details Test fixture for Quick Sort
 * 
 */
class QuickSortTest : public ::testing::Test {
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

// Benchmarking Function QuickSort Recursive vs Iterative
void benchmarkQuickSort() {
    std::vector<float> data1 = generateRandomNumbers<float>(200000, 1.0, 1000.0);
    std::vector<float> data2 = data1;

    // Recursive Quick Sort Timing
    auto start = std::chrono::high_resolution_clock::now();
    eden::quickSort<float>(data1);
    auto end = std::chrono::high_resolution_clock::now();
    double recursiveTime = std::chrono::duration<double, std::milli>(end - start).count();

    // Iterative Quick Sort Timing
    start = std::chrono::high_resolution_clock::now();
    eden::sort_iter::quickSort<float>(data1);
    end = std::chrono::high_resolution_clock::now();
    double iterativeTime = std::chrono::duration<double, std::milli>(end - start).count();

    // Display Results
    std::cout << "Data Size: " << data1.size() << " elements\n";
    std::cout << "Recursive Quick Sort Time: " << recursiveTime << " ms\n";
    std::cout << "Iterative Quick Sort Time: " << iterativeTime << " ms\n";
    std::cout << "---------------------------------------------\n";
}

TEST_F(QuickSortTest, BenchmarkQuickSort) {
    
    benchmarkQuickSort();
}

// Test sorting an unsorted vector
TEST_F(QuickSortTest, SortUnsortedVector) {
    std::vector<int> vec = {5, 3, 8, 1, 2};
    eden::quickSort<int>(vec);
    
    // Check that the vector is now sorted
    EXPECT_TRUE(isSorted<int>(vec));
}

// Test sorting an already sorted vector
TEST_F(QuickSortTest, SortSortedVector) {
    std::vector<int> vec = {1, 2, 3, 5, 8};
    eden::quickSort<int>(vec);
    
    // Check that the vector is still sorted
    EXPECT_TRUE(isSorted<int>(vec));
}
    
// Test sorting a vector with one element
TEST_F(QuickSortTest, SortSingleElement) {
    std::vector<int> vec = {42};
    eden::quickSort<int>(vec);
    
    // A single element is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting an empty vector
TEST_F(QuickSortTest, SortEmptyVector) {
    std::vector<int> vec = {};
    eden::quickSort<int>(vec);
    
    // An empty vector is trivially sorted
    EXPECT_TRUE(isSorted(vec));
}
    
// Test sorting a vector with duplicate elements
TEST_F(QuickSortTest, SortVectorWithDuplicates) {
    std::vector<int> vec = {4, 2, 5, 5, 1, 2, 3};
    eden::quickSort<int>(vec);
    
    // Check that the vector is sorted correctly, including duplicates
    EXPECT_TRUE(isSorted(vec));
}
    
TEST_F(QuickSortTest, SortRandomNumbers) {
    eden::quickSort<int>(randomNumbers);
    
    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomNumbers));
}

TEST_F(QuickSortTest, SortsFloatingPointNumbers) {
    eden::quickSort<float>(randomFloatNumbers);
    
    // Check that the vector is sorted correctly
    EXPECT_TRUE(isSorted(randomFloatNumbers));
}

TEST_F(QuickSortTest, SortsStrings) {
    std::vector<std::string> arr = {"banana", "apple", "cherry", "date"};
    eden::quickSort<std::string>(arr);
    EXPECT_TRUE(isSorted(arr));
}

// void selectionSortExample(std::vector<int>& arr, bool printFlag) {
//     std::cout << "SelectionSort: ";
//     if (printFlag) {
//         std::cout << "SelectionSort Unsorted array: ";
//         eden::printArray(arr);
//     }

//     eden::st_time_elapsed te;
//     te.time_start();
//     eden::selectionSort(arr);
//     te.time_elapsed_nano();

//     if (printFlag) {
//         std::cout << "SelectionSort Sorted array: ";
//         eden::printArray(arr);
//     }
// }