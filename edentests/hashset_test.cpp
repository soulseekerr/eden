
#include <gtest/gtest.h>
#include "hashset.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

/**
 * @brief HashSetTest class
 * @details Test fixture for HashSet_t class
 * 
 */
class HashSetTest : public ::testing::Test {
protected:
    HashSet_t<int> hashSet1;
    HashSet_t<uint64_t> hashSet2;
    HashSet_t<std::string, std::hash<std::string>> stringHashSet;

    void SetUp() override {
        hashSet1.insert(10);
        hashSet1.insert(20);
        hashSet1.insert(30);

        hashSet2.insert(1436596);
        hashSet2.insert(73875350870UL);
        hashSet2.insert(9707435087ULL);
        hashSet2.insert(430847508);
        hashSet2.insert(6586589);
        hashSet2.insert(1436536596);
        hashSet2.insert(675930);
        hashSet2.display();

        stringHashSet.insert("hello");
        stringHashSet.insert("world");
        stringHashSet.insert("vince");
        stringHashSet.insert("robert");
        stringHashSet.insert("anna");
        stringHashSet.display();
    }
};

// Test inserting and searching for elements
TEST_F(HashSetTest, InsertAndSearch) {
    EXPECT_TRUE(hashSet1.search(10));
    EXPECT_TRUE(hashSet1.search(20));
    EXPECT_TRUE(hashSet1.search(30));

    EXPECT_FALSE(hashSet1.search(40));  // Not inserted

    auto bFound1 = stringHashSet.search("vince");
    auto bFound2 = stringHashSet.search("Raf");
    std::cout << "vince->" << bFound1 << " Raf->" << bFound2 << std::endl;
}

// Test duplicate insertion handling
TEST_F(HashSetTest, DuplicateInsert) {
    EXPECT_FALSE(hashSet1.insert(10));  // Already exists
    EXPECT_FALSE(hashSet1.insert(20));  // Already exists
}

// Test removing elements
TEST_F(HashSetTest, RemoveElement) {
    EXPECT_TRUE(hashSet1.remove(20));   // Successfully removed
    EXPECT_FALSE(hashSet1.search(20));  // Should no longer exist
    EXPECT_FALSE(hashSet1.remove(20));  // Removing again should fail
}

// Test hash collisions (forces multiple elements into the same bucket)
TEST(HashSetCollisionTest, CollisionHandling) {
    HashSet_t<int> collisionSet;
    
    int val1 = 5;
    int val2 = 5 + 11; // 11 is the first prime bucket size, causing a collision

    EXPECT_TRUE(collisionSet.insert(val1));
    EXPECT_TRUE(collisionSet.insert(val2)); // Should still succeed
    EXPECT_TRUE(collisionSet.search(val1));
    EXPECT_TRUE(collisionSet.search(val2));

    EXPECT_TRUE(collisionSet.remove(val1));
    EXPECT_FALSE(collisionSet.search(val1));
    EXPECT_TRUE(collisionSet.search(val2));
}

// Test resizing behavior when exceeding load factor
TEST(HashSetResizeTest, ResizeOnLoadFactor) {
    HashSet_t<int> resizableSet(0.5); // Lower load factor to trigger resize early
    for (int i = 0; i < 20; i++) {
        resizableSet.insert(i);
    }

    // Ensure all elements are still present after resizing
    for (int i = 0; i < 20; i++) {
        EXPECT_TRUE(resizableSet.search(i));
    }
}

// Test insertion after resizing
TEST(HashSetResizeTest, InsertAfterResize) {
    HashSet_t<int> resizableSet(0.5);
    for (int i = 0; i < 25; i++) {
        resizableSet.insert(i);
    }

    // Ensure resizing didn't break insertions
    EXPECT_TRUE(resizableSet.insert(50));
    EXPECT_TRUE(resizableSet.search(50));
}


// Generate random MarketData objects
std::vector<MarketDataExample> generateData(size_t count) {
    std::vector<MarketDataExample> data;
    std::mt19937_64 rng{std::random_device{}()};
    std::uniform_real_distribution<double> dist(1.0, 1000.0);
    for (uint64_t i = 0; i < count; ++i) {
        data.emplace_back(i, dist(rng));
    }
    return data;
}

void benchmark_hashset_stl() {

    const size_t numElements = 1000000; // Number of elements for testing

    // Generate data
    auto data = generateData(numElements);

    // Test STL unordered_set
    auto total_start = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::high_resolution_clock::now();

    std::unordered_set<MarketDataExample, MarketDataExampleHash> stlSet;
    for (const auto& item : data) {
        stlSet.insert(item);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> customSetDuration = end - start;
    std::cout << "STL unordered_set time [insert]: " << customSetDuration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    for (const auto& item : data) {
        stlSet.find(item);
    }

    end = std::chrono::high_resolution_clock::now();
    customSetDuration = end - start;
    std::cout << "STL unordered_set time [search]: " << customSetDuration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    for (const auto& item : data) {
        stlSet.erase(item);
    }

    end = std::chrono::high_resolution_clock::now();
    customSetDuration = end - start;
    std::cout << "STL unordered_set time [remove]: " << customSetDuration.count() << " seconds" << std::endl;

    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> stlSetDuration = total_end - total_start;
    std::cout << "STL unordered_set time: " << stlSetDuration.count() << " seconds" << std::endl;
}

void benchmark_hashset_custom() {
    
    const size_t numElements = 1000000; // Number of elements for testing

    // Generate data
    auto data = generateData(numElements);

    // Test Custom HashSet
    auto total_start = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::high_resolution_clock::now();
    
    HashSet_t<MarketDataExample, MarketDataExampleHash> customSet;
    for (const auto& item : data) {
        customSet.insert(item);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> customSetDuration = end - start;
    std::cout << "Custom HashSet time [insert]: " << customSetDuration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    for (const auto& item : data) {
        customSet.search(item);
    }

    end = std::chrono::high_resolution_clock::now();
    customSetDuration = end - start;
    std::cout << "Custom HashSet time [search]: " << customSetDuration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    for (const auto& item : data) {
        customSet.remove(item);
    }

    end = std::chrono::high_resolution_clock::now();
    customSetDuration = end - start;
    std::cout << "Custom HashSet time [remove]: " << customSetDuration.count() << " seconds" << std::endl;

    auto total_end = std::chrono::high_resolution_clock::now();

    customSetDuration = total_end - total_start;
    std::cout << "Custom HashSet time: " << customSetDuration.count() << " seconds" << std::endl;
}

TEST(HashSetBenchmark, BasicAssertions) {

    // this is a comparison ok, but based on random data
    // the hash function will be good if relevant with the data.
    // so it may not be the best choice in terms of perf for the test here.

    benchmark_hashset_stl();

    benchmark_hashset_custom();
}