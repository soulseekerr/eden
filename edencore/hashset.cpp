
#include "hashset.h"

// Example Usage
void example_hashset_t() {
    HashSet_t<int> hashSet;
    
    // Insert elements
    hashSet.insert(15);
    hashSet.insert(25);
    hashSet.insert(35);
     
    // Display contents
    hashSet.display(); 
    
    HashSet_t<std::string, std::hash<std::string>> stringHashSet;
    stringHashSet.insert("hello");
    stringHashSet.insert("world");
    stringHashSet.display();

    // HashSet using default load factor 0.75
    HashSet_t<MarketDataExample, MarketDataExampleHash> marketDataSet;
    
    // Create some MarketData instances
    MarketDataExample data1(1, 100.0);
    MarketDataExample data2(2, 150.0);
    MarketDataExample data3(3, 200.0);
    
    // Insert MarketData instances into HashSet
    marketDataSet.insert(data1);
    marketDataSet.insert(data2);
    marketDataSet.insert(data3);
    
    // Display contents
    marketDataSet.display();
    
    // Search for an item
    marketDataSet.search(MarketDataExample(2, 150.0)); // Should find
    marketDataSet.search(MarketDataExample(4, 250.0)); // Should not find
    
    // Remove an item
    marketDataSet.remove(MarketDataExample(1, 100.0));
    marketDataSet.display();
}