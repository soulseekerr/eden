#include <gtest/gtest.h>
#include "logger.h"
#include <sstream>

// Demonstrate some basic assertions.
TEST(LoggerTest, BasicAssertions) {
    // LoggerManager test.
    auto& logManager1 = eden::LoggerManager::getInstance();
    logManager1.addLogger(std::make_unique<eden::LoggerConsole>(eden::LOG_LEVEL::LOG_DEBUG));
    logManager1.showInstanceAddress();

    auto& logManager2 = eden::LoggerManager::getInstance();
    logManager2.showInstanceAddress();

    EXPECT_EQ(&logManager1, &logManager2);
}