#pragma once

#include <iostream>
#include <vector>

// Function to print the elements of a vector
template<typename T>
void printArray(const std::vector<T>& arr) {
    for (T i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// Function to perform bubble sort on a vector of integers
template <typename T>
void bubbleSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // Last i elements are already in place
        for (int j = 0; j < n - i - 1; ++j) {
            // Swap if the element found is greater than the next element
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function to perform selection sort on a vector of integers
template <typename T>
void selectionSort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        // Find the minimum element in the unsorted portion
        int minIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the found minimum element with the first unsorted element
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}

// MergeSort: Function to merge two sorted subarrays
void merge(std::vector<int>& arr, int left, int mid, int right);

// Function to implement merge sort
void mergeSort(std::vector<int>& arr, int left, int right);

void mergeSortFull(std::vector<int>& arr);

// Function to swap two elements
void swap(int& a, int& b);

// Partition function to place the pivot element in its correct position
int partition(std::vector<int>& arr, int low, int high);

// QuickSort function
void quickSort(std::vector<int>& arr, int low, int high);

// Function to heapify a subtree rooted with node i which is an index in arr[]
void heapify(std::vector<int>& arr, int n, int i);

// Main function to do heap sort
void heapSort(std::vector<int>& arr);

void heapExample();

void quickSortExample(std::vector<int>& arr, bool printFlag);
void mergeSortExample(std::vector<int>& arr, bool printFlag);
void selectionSortExample(std::vector<int>& arr, bool printFlag);
void bubbleSortExample(std::vector<int>& arr, bool printFlag);

// C++ Example of Max-Heap Operations

class MaxHeap {
public:
    MaxHeap() {}

    // Function to insert a new value into the heap
    void insert(int val) {
        heap.push_back(val);
        heapifyUp(heap.size() - 1);
    }

    // Function to extract the maximum value (root) from the heap
    int extractMax() {
        if (heap.size() == 0) {
            throw std::runtime_error("Heap is empty");
        }
        int maxVal = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return maxVal;
    }

    // Function to print the elements of the heap
    void printHeap() const {
        for (int val : heap) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

private:
    std::vector<int> heap;

    // Function to maintain the heap property while inserting a new element
    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (heap[index] > heap[parentIndex]) {
                std::swap(heap[index], heap[parentIndex]);
                index = parentIndex;
            } else {
                break;
            }
        }
    }

    // Function to maintain the heap property while extracting the maximum element
    void heapifyDown(int index) {
        int n = heap.size();
        while (true) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;

            if (leftChild < n && heap[leftChild] > heap[largest]) {
                largest = leftChild;
            }
            if (rightChild < n && heap[rightChild] > heap[largest]) {
                largest = rightChild;
            }
            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }
};