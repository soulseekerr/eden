#pragma once

#include "singleton.h"
#include <vector>
#include <memory>
#include <string_view>
#include <fstream>
#include <exception>

namespace eden {

enum class LOG_LEVEL {
    LOG_ERROR = -1,
    LOG_INFO = 4,
    LOG_WARN = 3,
    LOG_DEBUG = 2,
    LOG_DEBUGVERBOSE = 1
};

class LoggerException : public std::exception {
public:
    explicit LoggerException(const std::string& message) 
        : message_(message) {}

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class Logger {

public:
    Logger(const LOG_LEVEL level);

    virtual ~Logger();

    void setLevel(const LOG_LEVEL level) { level_ = level; }

    LOG_LEVEL getLevel() const { return level_; }

    virtual void write(const std::string& s) = 0;

private:
    LOG_LEVEL     level_; 
};

class LoggerConsole : public Logger {

public:
    LoggerConsole(const LOG_LEVEL level);

    virtual ~LoggerConsole();

    void write(const std::string& s) override;
};

class LoggerFile : public Logger {

public:
    LoggerFile(const LOG_LEVEL level, const std::string& fileName);

    virtual ~LoggerFile();

    void write(const std::string& s) override;

private:
    std::ofstream   file_;
    std::string     fileName_;
};

/**
 * Manager of Loggers.
 */
class LoggerManager : public SingletonT<LoggerManager> {

    // make the current class friend of the Singleton class
    MAKE_SINGLETON(LoggerManager)

public:
    void showInstanceAddress() const;

    void addLogger(std::unique_ptr<Logger> logger);

    template<typename... Args>
    std::string dynaWriteGet(std::string_view rt_fmt_str, Args&&... args);

    template<typename... Args>
    void log(std::string_view rt_fmt_str, Args&&... args);

    template<typename... Args>
    void logLevel(LOG_LEVEL level, std::string_view rt_fmt_str, Args&&... args);

    template<typename... Args>
    void logInfo(std::string_view rt_fmt_str, Args&&... args);

    template<typename... Args>
    void logDebug(std::string_view rt_fmt_str, Args&&... args);

    template<typename... Args>
    void logDebugVerbose(std::string_view rt_fmt_str, Args&&... args);

    template<typename... Args>
    void logError(std::string_view rt_fmt_str, Args&&... args);

private:
    std::vector<std::unique_ptr<Logger>>  loggers_;
};


} // namespace eden