#pragma once

#include <vector>
#include <stack>
#include <iostream>
#include <thread>
#include <stop_token>

// Avoids deep recursion and stack overflow issues
// Better performance on large datasets by preventing function call overhead
// Ensures tail-recursive behavior, which compilers may not always optimize well in the recursive version

namespace eden {

namespace sort_iter_mt {

// Partition function for Quick Sort
template <typename T>
int partition(std::vector<T>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    
    // Median-of-Three: Pick median of {arr[low], arr[mid], arr[high]}
    // The median-of-three pivot selection prevents worst-case O(n²) behavior,
    // ensuring balanced partitions, and is O(n log n).
    if (arr[mid] < arr[low]) std::swap(arr[mid], arr[low]);
    if (arr[high] < arr[low]) std::swap(arr[high], arr[low]);
    if (arr[mid] < arr[high]) std::swap(arr[mid], arr[high]);

    T pivot = arr[high]; // New pivot
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Iterative Quick Sort function
template <typename T>
void quickSort(std::vector<T>& arr, size_t low, size_t high, size_t depth = 0) {
    if (arr.empty()) return;

    // vector slightly more efficient than stack (based on deque)
    // using push_back and pop_back improves cache locality

    // Vector to simulate stack behavior
    std::vector<std::pair<size_t, size_t>> stack;
    stack.push_back({low, high});

    while (!stack.empty()) {
        auto [start, end] = stack.back();
        stack.pop_back();

        if (start < end) {
            size_t pivotIndex = partition(arr, start, end);

            bool useThreads = (end - start) > THREAD_THRESHOLD && depth < std::thread::hardware_concurrency();

            if (useThreads) {
                // Launch parallel threads for sorting the subarrays
                std::jthread leftThread(quickSort<T>, std::ref(arr), start, pivotIndex - 1, depth + 1);
                std::jthread rightThread(quickSort<T>, std::ref(arr), pivotIndex + 1, end, depth + 1);

            } else {
                // Otherwise, push the subarrays onto the vector for further processing
                stack.push_back({pivotIndex + 1, end});
                stack.push_back({start, pivotIndex - 1});
            }
        }
    }
}

template <typename T>
void quickSort(std::vector<T>& arr) {
    if (arr.empty()) return;
    quickSort<T>(arr, 0, arr.size() - 1);
}

} // namespace sort_iter
} // namespace eden