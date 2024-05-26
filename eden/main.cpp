
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
    // Test fmt library
    fmt::print("Version: {}", getVersion());

    eden::st_datetime nowdt;
    auto logFile = std::format("eden_logger_{}.log", nowdt.year_month_day_h());

    // Log Manager
    auto& logManager = eden::LoggerManager::getInstance();
    logManager.addLogger(std::make_unique<eden::LoggerConsole>(eden::LOG_LEVEL::LOG_DEBUG));
    logManager.addLogger(std::make_unique<eden::LoggerFile>(eden::LOG_LEVEL::LOG_DEBUG, logFile));

    logManager.logInfo("Starting log - {}", eden::DateTime().toString());


    logManager.logInfo("Ending log - {}", eden::DateTime().toString());

    return 0;
}