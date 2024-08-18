
#include <gtest/gtest.h>

#include "pair.h"

#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>

class TestName {
public:
    TestName(const std::string& name) : name_(name) {}

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class TestA : public TestName {
public:
    TestA() : TestName("TestA") {}
    ~TestA() {}
};

class TestB : public TestName {
public:
    TestB() : TestName("TestB") {}
    ~TestB() {}
};

TEST(PairTest, BasicAssertions) {

    std::cout << "Testing Pair" << std::endl;

    Pair<TestA, TestB> pair1;

    std::cout << pair1.first.name() << std::endl;
    std::cout << pair1.second.name() << std::endl;

    EXPECT_EQ(pair1.first.name(), "TestA");
    EXPECT_EQ(pair1.second.name(), "TestB");
}
