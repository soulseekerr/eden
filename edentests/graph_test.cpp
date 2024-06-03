#include <gtest/gtest.h>
#include "graph.h"

using namespace eden;
using namespace eden::analytics;

// Demonstrate some basic assertions.
TEST(GraphTest, BasicAssertions) {

    std::vector<TreeNode> tasks = {
        TreeNode("Task1"),
        TreeNode("Task2"),
        TreeNode("Task3")
    };

    VectorTaskAggregate<TreeNode> aggregate(tasks);
    
    auto iterator = aggregate.createIterator();

    while (iterator->hasNext()) {
        auto it = iterator->next();
        std::cout << "Iterator on " << it.getName() << std::endl;
        it.DoStuff();
        it.DoOtherStuff();
    }
}