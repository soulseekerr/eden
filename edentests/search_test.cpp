
#include <gtest/gtest.h>

#include "sort.h"
#include "search.h"
#include "dfs.h"
#include "randomnumbers.h"
#include "redblacktree.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

using namespace eden;

TEST(SearchTest, BasicAssertions) {

    std::cout << "Testing Search Algorithms" << std::endl;

    heapExample();

    std::cout << "Testing Sorting Algorithms:" << std::endl;
    std::vector<int> arr = {10, 7, 8, 9, 1, 5, 45, 71, 4, 2};
    bubbleSortExample(arr, true);
    selectionSortExample(arr, true);
    mergeSortExample(arr, true);
    quickSortExample(arr, true);

    // Generate num random numbers between min and max
    int num = 10000;
    int min = 1;
    int max = 1000;
    std::vector<int> randomNumbers = generateRandomNumbers(num, min, max);
    bubbleSortExample(randomNumbers, false);
    selectionSortExample(randomNumbers, false);
    mergeSortExample(randomNumbers, false);
    quickSortExample(randomNumbers, false);

    // Generate num random numbers between min and max
    // num = 10000;
    // min = 1;
    // max = 1000;
    // randomNumbers = generateRandomNumbers(num, min, max);
    // bubbleSortExample(randomNumbers, false);
    // selectionSortExample(randomNumbers, false);
    // mergeSortExample(randomNumbers, false);
    // quickSortExample(randomNumbers, false);

    std::cout << "Testing RedBlackTree:" << std::endl;
    // Red Black Tree
    RedBlackTree rbTree;

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

