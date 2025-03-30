
#include <gtest/gtest.h>
#include "sort.h"
#include "randomnumbers.h"
#include "search.h"
#include "redblacktree.h"
#include "dfs.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

using namespace eden;

void heapExample() {
    eden::MaxHeap heap;
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

TEST(SearchTest, BasicAssertions) {

    std::cout << "Testing Search Algorithms" << std::endl;

    heapExample();

    std::cout << "Testing RedBlackTree:" << std::endl;
    // Red Black Tree
    eden::RedBlackTree rbTree;

    rbTree.insert(10);
    rbTree.insert(20);
    rbTree.insert(1);
    rbTree.insert(3);
    rbTree.insert(8);
    rbTree.insert(99);
    rbTree.insert(3);
    rbTree.insert(6);
    rbTree.insert(13);

    std::cout << "Inorder traversal of the Red-Black Tree:";
    rbTree.inorder();
    std::cout << std::endl;

    std::cout << "Testing Search:" << std::endl;

    int arr_search[] = {10, 23, 45, 70, 11, 15};
    int size = 6;
    int target = 70;
    linearSearchExample<int>(arr_search, size, target);

    int arr_binarysearch[] = {2, 3, 4, 10, 40};
    size = 5;
    target = 10;
    binarySearchExample(arr_binarysearch, size, target);

    std::cout << "DFS Example:" << std::endl;

    dfsExample();
}

