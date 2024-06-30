#pragma once

#include <iostream>

// Function to perform linear search
template <typename T>
int linearSearch(T* arr, size_t size, T target) {
    for (auto i = 0; i < size; i++) {

        if (arr[i] == target) {
            return i; // Return the index of the target element
        }
    }
    return -1; // Return -1 if the target element is not found
}

template <typename T>
void linearSearchExample(T* arr, int size, T target) {
    // Perform linear search
    int result = linearSearch<T>(arr, size, target);

    // Check if the element was found
    if (result != -1) {
        std::cout << "Element found at index " << result << std::endl;
    } else {
        std::cout << "Element not found in the array" << std::endl;
    }
}

// Function to perform binary search
int binarySearch(int arr[], int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2; // Calculate the mid index to avoid overflow

        // Check if target is present at mid
        if (arr[mid] == target)
            return mid;

        // If target is greater, ignore the left half
        if (arr[mid] < target)
            left = mid + 1;
        // If target is smaller, ignore the right half
        else
            right = mid - 1;
    }

    // Target is not present in array
    return -1;
}

void binarySearchExample(int* arr, int size, int target) {
    // Perform binary search
    int result = binarySearch(arr, 0, size - 1, target);

    // Check if the element was found
    if (result != -1) {
        std::cout << "Element found at index " << result << std::endl;
    } else {
        std::cout << "Element not found in the array" << std::endl;
    }
}
