
#include "logger.h"
#include <string>
#include <format>
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

    file_ << "EDEN Log file." << std::endl;
}

LoggerFile::~LoggerFile() {}

void LoggerFile::write(const std::string& s) {}

void LoggerManager::addLogger(std::unique_ptr<Logger> logger) {
    loggers_.push_back(std::move(logger));
}

void LoggerManager::showInstanceAddress() const {
    std::cout << "LoggerManager instance: " << this << std::endl;
}

template<typename... Args>
std::string LoggerManager::dynaWriteGet(std::string_view rt_fmt_str, Args&&... args) {
    
    return std::vformat(rt_fmt_str, std::make_format_args(args...));
}

template<typename... Args>
void LoggerManager::log(std::string_view rt_fmt_str, Args&&... args) {

    auto str = dynaWriteGet(rt_fmt_str, std::make_format_args(args...));
    
    for (std::unique_ptr<Logger>& logger : loggers_) {
        logger->write(str);
    }
}

template<typename... Args>
void LoggerManager::logLevel(LOG_LEVEL level, std::string_view rt_fmt_str, Args&&... args) {

    auto str = dynaWriteGet(rt_fmt_str, std::make_format_args(args...));
    
    for (std::unique_ptr<Logger>& logger : loggers_) {
        if (logger->getLevel() >= level)
            logger->write(str);
    }
}

template<typename... Args>
void LoggerManager::logInfo(std::string_view rt_fmt_str, Args&&... args) {

    logLevel(LOG_LEVEL::LOG_INFO, rt_fmt_str, std::make_format_args(args...));
}

template<typename... Args>
void LoggerManager::logDebug(std::string_view rt_fmt_str, Args&&... args) {

    logLevel(LOG_LEVEL::LOG_DEBUG, rt_fmt_str, std::make_format_args(args...));
}

template<typename... Args>
void LoggerManager::logDebugVerbose(std::string_view rt_fmt_str, Args&&... args) {

    logLevel(LOG_LEVEL::LOG_DEBUGVERBOSE, rt_fmt_str, std::make_format_args(args...));
}

template<typename... Args>
void LoggerManager::logError(std::string_view rt_fmt_str, Args&&... args) {

    logLevel(LOG_LEVEL::LOG_ERROR, rt_fmt_str, std::make_format_args(args...));
}