
#include "logger.h"
#include "datetime.h"
#include <iostream>

using namespace eden;


Logger::Logger(const LOG_LEVEL level) : level_(level) {}

Logger::~Logger() {}

LoggerConsole::LoggerConsole(const LOG_LEVEL level) : Logger(level) {}

LoggerConsole::~LoggerConsole() {}

void LoggerConsole::write(const std::string& s) {
    std::cout << s << std::endl;
}

LoggerFile::LoggerFile(const LOG_LEVEL level, const std::string& fileName) 
    : Logger(level) {

    fileName_ = fileName;

    std::ofstream myLog(fileName_.c_str());
    file_ = std::move(myLog);

    if (!file_)
        throw LoggerException("Error: The log file was not opened.");

    // File is opened
    file_ << "EDEN Log file." << std::endl;
}

LoggerFile::~LoggerFile() {}

void LoggerFile::write(const std::string& s) {
    file_ << s << std::endl;
}

void LoggerManager::addLogger(std::unique_ptr<Logger> logger) {
    loggers_.push_back(std::move(logger));
}

void LoggerManager::showInstanceAddress() const {
    std::cout << "LoggerManager instance: " << this << std::endl;
}