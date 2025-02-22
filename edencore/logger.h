#pragma once
// Helvetica Light
#include "log_levels.h"
#include "singleton.h"
#include <vector>
#include <memory>
#include <string_view>
#include <fstream>
#include <exception>
#include <format>

namespace eden {

// X Macro for log levels with color
enum class LOG_LEVEL {
#define X(name, color, str) name,
    LOG_LEVELS
#undef X
};

inline const char* getLogLevelString(LOG_LEVEL level) {
    switch (level) {
#define X(name, color, str) case LOG_LEVEL::name: return str;
        LOG_LEVELS
#undef X
        default: return "UNKNOWN";
    }
}

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

    virtual void write(const LOG_LEVEL level, const std::string& s) = 0;

private:
    LOG_LEVEL     level_; 
};

// Simple Logger for console output 
class LoggerConsole : public Logger {

public:
    LoggerConsole(const LOG_LEVEL level);

    virtual ~LoggerConsole();

    void write(const LOG_LEVEL level, const std::string& s) override;
};

// Logger with simple file output 
class LoggerFile : public Logger {

public:
    LoggerFile(const LOG_LEVEL level, const std::string& fileName);

    virtual ~LoggerFile();

    void write(const LOG_LEVEL level, const std::string& s) override;

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
            logger->write(LOG_LEVEL::LOG_INFO, str);
        }
    }

    template<typename... Args>
    void logLevel(LOG_LEVEL level, std::string_view rt_fmt_str, Args&&... args) {

        auto str = std::vformat(rt_fmt_str, std::make_format_args(args...));

        for (std::unique_ptr<Logger>& logger : loggers_) {
            if (level >= logger->getLevel())
                logger->write(level, str);
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

        logLevel(LOG_LEVEL::LOG_WARNING, rt_fmt_str, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void logError(std::string_view rt_fmt_str, Args&&... args) {

        logLevel(LOG_LEVEL::LOG_ERROR, rt_fmt_str, std::forward<Args>(args)...);
    }

private:
    std::vector<std::unique_ptr<Logger>>  loggers_;
};


} // namespace eden