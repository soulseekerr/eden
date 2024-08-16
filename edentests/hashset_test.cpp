
#include <gtest/gtest.h>
// #include "gd_hashset.h"
#include "hashset.h"

#include <iostream>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>


TEST(HashSetTest, BasicAssertions) {
    // HashSet<int> hs1;
    // hs1.insert(1234);

    HashSet hashSet;
    
    // Insert elements
    hashSet.insert(15);
    hashSet.insert(25);
    hashSet.insert(35);
    hashSet.insert(20);
    hashSet.insert(30);
    hashSet.insert(25);
    hashSet.insert(100);
    hashSet.insert(200);
    hashSet.insert(300);
    hashSet.insert(400);
    hashSet.insert(500);
    
    // Display contents
    hashSet.display();
    
    // Search for elements
    hashSet.search(20);
    hashSet.search(100);
    
    // Remove elements
    hashSet.remove(25);
    hashSet.remove(100);
    
    // Display contents after removal
    hashSet.display();

    std::cout << "Testing HashSet_t" << std::endl;

    HashSet_t<uint64_t> hs2;

    hs2.insert(1436596);
    hs2.insert(73875350870UL);
    hs2.insert(9707435087ULL);
    hs2.insert(430847508);
    hs2.insert(6586589);
    hs2.insert(1436536596);
    hs2.insert(675930);
    hs2.display();

    // HashSet using default std::hash function
    HashSet_t<std::string, std::hash<std::string>> stringHashSet;
    stringHashSet.insert("hello");
    stringHashSet.insert("world");
    stringHashSet.insert("vince");
    stringHashSet.insert("robert");
    stringHashSet.insert("anna");

    stringHashSet.display();

    auto bFound1 = stringHashSet.search("vince");
    auto bFound2 = stringHashSet.search("Raf");
    std::cout << "vince: " << bFound1 << " Raf: " << bFound2 << std::endl;


    std::cout << "Testing HashSet_t loade factor 0.5" << std::endl;
    
    // HashSet using custom load factor 0.5
    HashSet_t<uint64_t> intHashSet2(0.5);
    intHashSet2.insert(45);
    intHashSet2.insert(55);
    intHashSet2.insert(65);
    intHashSet2.display();
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

    benchmark_hashset_stl();

    benchmark_hashset_custom();
}