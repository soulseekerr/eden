#pragma once

#include <vector>
#include <thread>
#include <algorithm>
#include <iostream>
#include <thread>
#include <stop_token>

// If the partition size is large (THREAD_THRESHOLD),
// and the recursion depth is within CPU limits (std::hardware_concurrency()), we spawn a std::jthread.
// Otherwise, we perform regular recursive calls.
// Plus prevents spawning excessive threads

constexpr size_t THREAD_THRESHOLD = 50000; // Minimum size to parallelize

namespace eden {

namespace sort_mt {

// Median-of-Three Pivot Selection
template <typename T>
int medianOfThree(std::vector<T>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[right] < arr[left]) std::swap(arr[left], arr[right]);
    if (arr[mid] < arr[left]) std::swap(arr[mid], arr[left]);
    if (arr[right] < arr[mid]) std::swap(arr[right], arr[mid]);
    std::swap(arr[mid], arr[right - 1]);
    return right - 1;
}

// Partition function
template <typename T>
size_t partition(std::vector<T>& arr, size_t low, size_t high) {
    size_t pivotIndex = medianOfThree(arr, low, high);
    T pivot = arr[pivotIndex];
    size_t i = low;
    for (size_t j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr[pivotIndex]);
    return i;
}

// Parallel QuickSort
template <typename T>
void quickSort(std::vector<T>& arr, size_t low, size_t high, int depth = 0) {
    if (low >= high) {
        return;
    }
    
    size_t pivotIndex = partition(arr, low, high);

    bool useThreads = (high - low) > THREAD_THRESHOLD && depth < std::thread::hardware_concurrency();

    if (useThreads) {
        std::jthread leftThread(quickSort<T>, std::ref(arr), low, pivotIndex - 1, depth + 1);
        quickSort(arr, pivotIndex + 1, high, depth + 1);
    } else {
        quickSort(arr, low, pivotIndex - 1, depth);
        quickSort(arr, pivotIndex + 1, high, depth);
    }
}

// Helper function to start the sorting
template <typename T>
void quickSort(std::vector<T>& arr) {
    quickSort<T>(arr, 0, arr.size() - 1);
}

} // namespace sort_mt
} // namespace eden