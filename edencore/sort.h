#pragma once

#include <iostream>
#include <vector>

namespace eden {

// Function to print the elements of a vector
template<typename T>
void printArray(const std::vector<T>& arr) {
    for (T i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}


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

} // namespace eden