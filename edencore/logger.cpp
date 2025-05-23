
#include "logger.h"
#include "datetime.h"
#include <iostream>

using namespace eden;


Logger::Logger(const LOG_LEVEL level) : level_(level) {}

Logger::~Logger() {}

LoggerConsole::LoggerConsole(const LOG_LEVEL level) : Logger(level) {}

LoggerConsole::~LoggerConsole() {}

void LoggerConsole::write(const LOG_LEVEL level, const std::string& s) {

    std::string color_code;
    std::string reset_code = "\033[0m";

    // using the X Macro
    switch (level) {
#define X(name, color, str) case LOG_LEVEL::name: color_code = color; break;
        LOG_LEVELS
#undef X
        default: color_code = ""; break;
    }

    auto level_code = std::format("[{}, {}] ", getLogLevelString(level), DateTime().timeToString());

    auto str = color_code + level_code + s + reset_code;

    std::cout << str << '\n';
}

LoggerFile::LoggerFile(const LOG_LEVEL level, const std::string& fileName) 
    : Logger(level) {

    fileName_ = fileName;

    std::ofstream myLog(fileName_.c_str());
    file_ = std::move(myLog);

    if (!file_)
        throw LoggerException("Error: The log file was not opened.");

    // File is opened
    file_ << "EDEN Log file." << '\n';
}

LoggerFile::~LoggerFile() {}

void LoggerFile::write(const LOG_LEVEL level, const std::string& s) {
    file_ << DateTime().toString() << ": " << s << '\n';
}

void LoggerManager::addLogger(std::unique_ptr<Logger> logger) {
    loggers_.push_back(std::move(logger));
}

void LoggerManager::showInstanceAddress() const {
    std::cout << "LoggerManager instance: " << this << '\n';
}