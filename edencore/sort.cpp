
#include "sort.h"
#include "time_elapsed.h"

// MergeSort: Function to merge two sorted subarrays
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    std::vector<int> L(n1);
    std::vector<int> R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = arr[mid + 1 + i];

    // Merge the temporary arrays back into arr[left..right]

    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Function to implement merge sort
void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

void mergeSortFull(std::vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

// Function to swap two elements
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Partition function to place the pivot element in its correct position
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Pivot element is taken as the last element
    int i = low - 1; // Index of smaller element

    for (int j = low; j <= high - 1; ++j) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            ++i; // Increment index of smaller element
            swap(arr[i], arr[j]); // Swap the current element with the element at index i
        }
    }
    swap(arr[i + 1], arr[high]); // Place the pivot element in the correct position
    return i + 1;
}

// QuickSort function
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        // Partitioning index
        int pi = partition(arr, low, high);

        // Recursively sort elements before and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to heapify a subtree rooted with node i which is an index in arr[]
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // Left child
    int right = 2 * i + 2; // Right child

    // If left child is larger than root
    if (left < n && arr[left] > arr[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right] > arr[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// Main function to do heap sort
void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}


void heapExample() {
    MaxHeap heap;
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(6);
    heap.insert(1);
    heap.insert(8);
    heap.insert(9);

    std::cout << "Heap elements: ";
    heap.printHeap();

    std::cout << "Extracting max: " << heap.extractMax() << std::endl;
    std::cout << "Heap elements after extraction: ";
    heap.printHeap();
}

void quickSortExample(std::vector<int>& arr, bool printFlag) {
    std::cout << "QuickSort: ";
    if (printFlag) {
        std::cout << "QuickSort Unsorted array: ";
        printArray<int>(arr);
    }

    eden::st_time_elapsed te;
    te.time_start();
    quickSort(arr, 0, arr.size() - 1);
    te.time_elapsed_nano();

    if (printFlag) {
        std::cout << "QuickSort Sorted array: ";
        printArray<int>(arr);
    }
}

void mergeSortExample(std::vector<int>& arr, bool printFlag) {
    std::cout << "MergeSort: ";
    if (printFlag) {
        std::cout << "MergeSort Unsorted array: ";
        printArray<int>(arr);
    }

    eden::st_time_elapsed te;
    te.time_start();
    mergeSort(arr, 0, arr.size() - 1);
    te.time_elapsed_nano();

    if (printFlag) {
        std::cout << "MergeSort Sorted array: ";
        printArray<int>(arr);
    }
}

void selectionSortExample(std::vector<int>& arr, bool printFlag) {
    std::cout << "SelectionSort: ";
    if (printFlag) {
        std::cout << "SelectionSort Unsorted array: ";
        printArray(arr);
    }

    eden::st_time_elapsed te;
    te.time_start();
    selectionSort(arr);
    te.time_elapsed_nano();

    if (printFlag) {
        std::cout << "SelectionSort Sorted array: ";
        printArray(arr);
    }
}

void bubbleSortExample(std::vector<int>& arr, bool printFlag) {
    std::cout << "BubbleSort: ";
    if (printFlag) {
        std::cout << "BubbleSort Unsorted array: ";
        printArray(arr);
    }

    eden::st_time_elapsed te;
    te.time_start();
    bubbleSort(arr);
    te.time_elapsed_nano();

    if (printFlag) {
        std::cout << "BubbleSort Sorted array: ";
        printArray(arr);
    }
}