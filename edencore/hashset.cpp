
#include "hashset.h"

// Thomas Wang's 64-bit to 32-bit hash function
uint32_t HashSet::getHash(uint64_t key) {
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key = key ^ (key >> 31);
    key = key * 21; // key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);
    return static_cast<uint32_t>(key % bucketCount);
}

// Constructor
HashSet::HashSet() : elementCount(0), currentPrimeIndex(0) {
    bucketCount = primeSizes[currentPrimeIndex];
    buckets = std::make_unique<std::unique_ptr<Node>[]>(bucketCount);
}

// Resize function to move to the next prime size
void HashSet::resize() {
    if (currentPrimeIndex + 1 >= primeSizes.size()) return; // No more primes available

    int oldBucketCount = bucketCount;
    bucketCount = primeSizes[++currentPrimeIndex];

    auto newBuckets = std::make_unique<std::unique_ptr<Node>[]>(bucketCount);

    for (int i = 0; i < oldBucketCount; ++i) {
        Node* current = buckets[i].get();
        while (current) {
            int newHashValue = getHash(current->key);
            auto newNode = std::make_unique<Node>(current->key);
            newNode->next = std::move(newBuckets[newHashValue]);
            newBuckets[newHashValue] = std::move(newNode);
            current = current->next.get();
        }
    }

    buckets = std::move(newBuckets);
}

// Insert Key
bool HashSet::insert(uint64_t key) {
    if (elementCount > bucketCount * 0.7) {
        resize(); // Resize if load factor > 0.7
    }

    uint32_t hashValue = getHash(key);
    Node* current = buckets[hashValue].get();
    
    while (current != nullptr) {
        if (current->key == key) {
            // std::cout << "Key " << key << " already exists. Insertion skipped." << std::endl;
            return false;
        }
        current = current->next.get();
    }

    auto newNode = std::make_unique<Node>(key);
    newNode->next = std::move(buckets[hashValue]);
    buckets[hashValue] = std::move(newNode);
    ++elementCount;
    // std::cout << "Key " << key << " inserted." << std::endl;
    return true;
}

// Search Key
bool HashSet::search(uint64_t key) {
    uint32_t hashValue = getHash(key);
    Node* current = buckets[hashValue].get();
    
    while (current != nullptr) {
        if (current->key == key) {
            // std::cout << "Key " << key << " found." << std::endl;
            return true;
        }
        current = current->next.get();
    }
    
    // std::cout << "Key " << key << " not found." << std::endl;
    return false;
}

// Remove Key
bool HashSet::remove(uint64_t key) {
    uint32_t hashValue = getHash(key);
    Node* current = buckets[hashValue].get();
    Node* prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                buckets[hashValue] = std::move(current->next);
            } else {
                prev->next = std::move(current->next);
            }
            // std::cout << "Key " << key << " removed." << std::endl;
            --elementCount;
            return true;
        }
        prev = current;
        current = current->next.get();
    }
    
    std::cout << "Key " << key << " not found. Removal skipped." << std::endl;
    return false;
}

// Display HashSet
void HashSet::display() {
    std::cout << "HashSet contents:" << std::endl;
    for (int i = 0; i < bucketCount; ++i) {
        std::cout << "Bucket " << i << ": ";
        Node* current = buckets[i].get();
        while (current != nullptr) {
            std::cout << current->key << " -> ";
            current = current->next.get();
        }
        std::cout << "nullptr" << std::endl;
    }
}

// Example Usage
void example_hashset() {
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
}

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