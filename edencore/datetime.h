#pragma once

/**
Introduced in C++11 and significantly improved in C++20, 
it provides a robust and flexible way to handle date and time.
*/
#include <chrono>
#include <format>  // for C++20
#include <iostream>
#include <ctime>
#include <regex>
#include <array>
#include <utility>
#include <cstdlib>
#include <iomanip>
#include <cassert>

using namespace std::literals; // enables literal suffixes, e.g. 24h, 1ms, 1s.

namespace eden {

    struct Format {
        const std::tm& tm;
        const char* formatStr;
        const std::locale& locale; // <- reference, always provided

        friend std::ostream& operator<<(std::ostream& os, const Format& f) {
            std::ostream::sentry s(os);
            if (s) {
                std::locale oldLocale = os.getloc();
                os.imbue(f.locale);
                os << std::put_time(&f.tm, f.formatStr);
                os.imbue(oldLocale);
            }
            return os;
        }
    };

/** 
 * @brief DateTime class
 * @details This class is used to represent date and time.
 * It provides various methods to manipulate and format date and time.
 * It uses std::chrono for time representation and std::tm for formatting.
 */
class DateTime {
public:
    // Default constructor that initializes to the current time
    DateTime() : time_point_(std::chrono::system_clock::now()), dirty_(true) {}

    // Constructor accepting year, month, and day
    DateTime(int year, int month, int day) : time_point_(std::chrono::system_clock::from_time_t(
        [year, month, day]() -> std::time_t {
            assert(year >= 0 && "Invalid date passed to DateTime constructor.");
            assert(month >= 0 && "Invalid date passed to DateTime constructor.");
            assert(day >= 0 && "Invalid date passed to DateTime constructor.");

            // Convert std::tm to time_t, then to std::chrono::system_clock::time_point
            std::tm tm = {};
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            std::time_t time = std::mktime(&tm);
            assert(time != -1 && "Invalid date passed to DateTime constructor.");
            return time != -1 ? time : 0;
        }()
    )), dirty_(true) {}

    // Copy constructor
    DateTime(const DateTime& dt) = default;
    // Move constructor
    DateTime(DateTime&& dt) noexcept = default;
    virtual ~DateTime() = default;

    DateTime& operator=(const DateTime&) = default;
    DateTime& operator=(DateTime&&) noexcept = default;

    // Equality operator
    [[nodiscard]] bool operator==(const DateTime& dt) const {
        return (time_point_ == dt.time_point_);
    }

    [[nodiscard]] const std::string& toString() const {
        if (dirty_) {
            cached_string_ = formatTime("%Y-%m-%d %H:%M:%S");
            dirty_ = false;
        }
        return cached_string_;
    }

    [[nodiscard]] std::tm timepointToLocalTime() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        // seems not working - C++20 not fully settled on MacOS
        // return std::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&in_time_t));
        std::tm tm = *std::localtime(&in_time_t);
        return tm;
    }

    [[nodiscard]] std::string format(const char* formatStr) const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
    
        std::ostringstream oss;
        oss << Format{tm, formatStr, DateTime::getDefaultLocale()}; // std::put_time(&tm, formatStr);
        return oss.str();
    }

    [[nodiscard]] std::string timeToString() const {
        return format("%H:%M:%S");
    }

    [[nodiscard]] std::string year_month_day_h() const {
        return format("%Y-%m-%d");
    }

    [[nodiscard]] std::string year_month_day() const {
        return format("%Y%m%d");
    }

    [[nodiscard]] std::string year_month_h() const {
        return format("%Y-%m");
    }

    [[nodiscard]] std::string year_month() const {
        return format("%Y%m");
    }

    std::string year() const {
        return format("%Y");
    }

    void currentTime() const {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Current time: " << std::ctime(&now_time) << std::endl;
    }

    void addSeconds(int seconds);
    void subtractSeconds(int seconds);
    void addDays(int days);
    void subtractDays(int days);
    void addMonths(int months);
    void subtractMonths(int months);
    void addYears(int years);
    void subtractYears(int years);
    void addBusinessDays(int days);
    void subtractBusinessDays(int days);
    bool isBusinessDay() const;

    static const std::locale& getDefaultLocale() {
        static std::locale locale("en_US.UTF-8");
        return locale;
    }

    static void setDefaultLocale(const std::locale& loc) {
        const_cast<std::locale&>(getDefaultLocale()) = loc;
    }

private:
    // toTm Method: Converts the time_point_ to a std::tm structure.
    std::tm toTm() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        return tm;
    }

    // normalizeTm Method: Uses std::mktime to normalize the std::tm structure, 
    // which handles overflow and underflow of months and years.
    void normalizeTm(std::tm& tm) const {
        // Normalize the time structure (e.g., handle overflow of months into years)
        std::mktime(&tm);
    }

    // updateFromTm Method: Converts a normalized std::tm structure back to time_point_.
    void updateFromTm(const std::tm& tm) {
        // Convert the normalized tm back to time_point_
        std::time_t time = std::mktime(const_cast<std::tm*>(&tm));
        if (time == -1) {
            throw std::invalid_argument("Invalid date after normalization");
        }
        time_point_ = std::chrono::system_clock::from_time_t(time);
    }

    void markDirty() {
        dirty_ = true;
    }

    [[nodiscard]] std::string formatTime(const char* format) const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&tm, format);
        return oss.str();
    }

private:
    std::chrono::system_clock::time_point time_point_;
    // Allows to modify the variable in const methods
    // This is used to cache the string representation of the DateTime object
    mutable bool dirty_ = true;
    mutable std::string cached_string_;
};

} // namespace eden

