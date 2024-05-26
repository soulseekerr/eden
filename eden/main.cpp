
#include "ver.h"
#include "hello.h"
#include "logger.h"
#include "time_elapsed.h"
#include "datetime.h"
#include <sstream>
#include <format>
#include <string>
#include <fmt/core.h>
#include <iostream>

/**
 * Return EDEN version.
 *
 */
static const std::string getVersion()
{
	std::ostringstream lFlux;
	lFlux << "EDEN Core v" << EDEN_CORE_MAJOR_VERSION << "." << EDEN_CORE_MINOR_VERSION << "." << EDEN_CORE_REVISION_VERSION << std::endl;
	return lFlux.str();
}

int main() {

    // Test function call in library
    hello();

    // Test fmt library
    fmt::print("Versions: {}", getVersion());

    eden::st_datetime dt;
    std::cout << dt.year_month_day_h() << "\n";

    std::string logFile = std::format("eden_logger_{}.log", dt.year_month_day_h());

    auto& logManager = eden::LoggerManager::getInstance();

    logManager.addLogger(std::make_unique<eden::LoggerConsole>(eden::LOG_LEVEL::LOG_DEBUG));
    logManager.addLogger(std::make_unique<eden::LoggerFile>(eden::LOG_LEVEL::LOG_DEBUG, logFile));

    // TESTS
    eden::DateTime cobDt;
    std::cout << cobDt.year_month_day_h() << "\n";

    eden::DateTime nowDt = eden::DateTime::Now();
    std::cout << "Now Dt year_month_day_h: " << nowDt.year_month_day_h() << "\n";
    // TESTS

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

    return 0;
}