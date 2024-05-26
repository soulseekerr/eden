#pragma once

#include "singleton.h"
#include <vector>
#include <memory>
#include <string_view>
#include <fstream>
#include <exception>
#include <format>

namespace eden {

// Different levels of logs
enum class LOG_LEVEL {
    LOG_ERROR = 5,
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

// Abstract Logger class
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

// Simple Logger for console output 
class LoggerConsole : public Logger {

public:
    LoggerConsole(const LOG_LEVEL level);

    virtual ~LoggerConsole();

    void write(const std::string& s) override;
};

// Logger with simple file output 
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
    std::string dynaWriteGet(std::string_view rt_fmt_str, Args&&... args) {
        
        return std::vformat(rt_fmt_str, std::make_format_args(args...));
    }

    template<typename... Args>
    void log(std::string_view rt_fmt_str, Args&&... args) {

        auto str = std::vformat(rt_fmt_str, std::make_format_args(args...));
        
        for (std::unique_ptr<Logger>& logger : loggers_) {
            logger->write(str);
        }
    }

    template<typename... Args>
    void logLevel(LOG_LEVEL level, std::string_view rt_fmt_str, Args&&... args) {

        auto str = std::vformat(rt_fmt_str, std::make_format_args(args...));

        std::string color_code;
        std::string reset_code = "\033[0m";

        switch (level) {
            case LOG_LEVEL::LOG_DEBUG:
                color_code = "\033[32m"; // Green
                break;
            case LOG_LEVEL::LOG_DEBUGVERBOSE:
                color_code = "\033[32m"; // Green
                break;
            case LOG_LEVEL::LOG_INFO:
                color_code = "\033[34m"; // Blue
                break;
            case LOG_LEVEL::LOG_ERROR:
                color_code = "\033[31m"; // Red
                break;
            case LOG_LEVEL::LOG_WARN:
                color_code = "\033[38;5;214m"; // Orange (approximate)
                break;
            default:
                color_code = "";
                break;
        }

        str = color_code + str + reset_code;

        for (std::unique_ptr<Logger>& logger : loggers_) {
            if (level >= logger->getLevel())
                logger->write(str);
        }
    }

    template<typename... Args>
    void logInfo(std::string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_INFO, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logDebug(std::string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_DEBUG, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logDebugVerbose(std::string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_DEBUGVERBOSE, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logWarn(std::string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_WARN, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logError(std::string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_ERROR, rt_fmt_str, std::forward<Args>(args)...);
    }

private:
    std::vector<std::unique_ptr<Logger>>  loggers_;
};


} // namespace eden