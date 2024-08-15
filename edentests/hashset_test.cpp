
#include <gtest/gtest.h>
// #include "gd_hashset.h"
#include "hashset.h"

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


    // HashSet using custom load factor 0.5
    // HashSet_t<uint64_t> intHashSet2(0.5);
    // intHashSet2.insert(45);
    // intHashSet2.insert(55);
    // intHashSet2.insert(65);
    // intHashSet2.display();
}