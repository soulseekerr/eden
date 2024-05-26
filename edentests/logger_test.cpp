#include <gtest/gtest.h>
#include "logger.h"
#include <sstream>

// Demonstrate some basic assertions.
TEST(LoggerTest, BasicAssertions) {
    // LoggerManager test.
    auto& logManager = eden::LoggerManager::getInstance();
    logManager.addLogger(std::make_unique<eden::LoggerConsole>(eden::LOG_LEVEL::LOG_DEBUG));
    logManager.showInstanceAddress();

    auto& logManager2 = eden::LoggerManager::getInstance();
    logManager2.showInstanceAddress();

    EXPECT_EQ(&logManager, &logManager2);

    // check that it runs well in the console
    std::cout << logManager.dynaWriteGet("This is an example 1 of log with dynaWriteGet with argument {}", 87) << std::endl;
    logManager.log("This is an example 2 of log method with argument {}", 55);
    logManager.logLevel(eden::LOG_LEVEL::LOG_INFO, "This is an example 3 of logLevel INFO with argument {}", 32);
    logManager.logLevel(eden::LOG_LEVEL::LOG_DEBUGVERBOSE, "This is an example 3b DEBUGVERBOSE not displayed of log Info with argument {}", -11);
    logManager.logLevel(eden::LOG_LEVEL::LOG_ERROR, "This is an ERROR example 4 ERROR with argument {}", -11);
    logManager.logLevel(eden::LOG_LEVEL::LOG_WARNING, "This is a WaRNING example 5 WARN with argument {}", 121);
    logManager.logLevel(eden::LOG_LEVEL::LOG_DEBUG, "This is an example 6  of log DEBUG with argument {}", "DEbug session");
    logManager.logInfo("This is an example 7 of logInfo with argument {}", 55);
    logManager.logWarn("This is an example 8 of logWarn with argument {}", 55);
    logManager.logError("This is an example 9 of logError with argument {}", 55);
    logManager.logInfo("Example 10 of log without argument");
}