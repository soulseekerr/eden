#pragma once

/*
Introduced in C++11 and significantly improved in C++20, 
it provides a robust and flexible way to handle date and time.
*/
#include <chrono>
#include <iostream>
#include <format>  // for C++20

#include<ctime>
#include<regex>
#include<array>
#include<utility>
#include<cstdlib>

using namespace std::literals; // enables literal suffixes, e.g. 24h, 1ms, 1s.

namespace eden {

// Month Enum which starts with 1.
enum class Month {
    Jan = 1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
};

// Datastructure for string to num conversion in month(.i.e."Mar" Month to 3)
// std::array<std::pair<std::string, int>, 12> monthinfo = {
//     std::make_pair(std::string("Jan"), Month::Jan),
//     std::make_pair(std::string("Feb"), Month::Feb),
//     std::make_pair(std::string("Mar"), Month::Mar),
//     std::make_pair(std::string("Apr"), Month::Apr),
//     std::make_pair(std::string("May"), Month::May),
//     std::make_pair(std::string("Jun"), Month::Jun),
//     std::make_pair(std::string("Jul"), Month::Jul),
//     std::make_pair(std::string("Aug"), Month::Aug),
//     std::make_pair(std::string("Sep"), Month::Sep),
//     std::make_pair(std::string("Oct"), Month::Oct),
//     std::make_pair(std::string("Nov"), Month::Nov),
//     std::make_pair(std::string("Dec"), Month::Dec)
// };

class DateTime {
public:
    // Default constructor that initializes to the current time
    DateTime() : time_point_(std::chrono::system_clock::now()) {}

    // Constructor accepting year, month, and day
    DateTime(int year, int month, int day) {
        std::tm tm = {};
        tm.tm_year = year - 1900;  // std::tm uses years since 1900
        tm.tm_mon = month - 1;     // std::tm months are 0-11
        tm.tm_mday = day;
        
        // Convert std::tm to time_t, then to std::chrono::system_clock::time_point
        std::time_t time = std::mktime(&tm);
        if (time == -1) {
            throw std::invalid_argument("Invalid date");
        }
        time_point_ = std::chrono::system_clock::from_time_t(time);
    }

    // Copy constructor
    DateTime(const DateTime& dt) {
        time_point_ = dt.time_point_;
    }

    // Move constructor
    DateTime(DateTime&& dt) noexcept {
        time_point_ = dt.time_point_;
    }

    virtual ~DateTime() {}

    // Assignment operator
    DateTime& operator=(const DateTime& dt) {
        if (this == &dt) {
            return *this;  // self-assignment
        }
        time_point_ = dt.time_point_;
        return *this;
    }

    // Move assignment operator
    DateTime& operator=(DateTime&& dt) {
        if (this == &dt) {
            return *this;  // self-assignment
        }
        time_point_ = dt.time_point_;
        return *this;
    }

    // Equality operator
    bool operator==(const DateTime& dt) {
        return (time_point_ == dt.time_point_);
    }

    // static DateTime Now() {
    //     auto tp = std::chrono::system_clock::now();
    //     time_t cstyle_t = std::chrono::system_clock::to_time_t(tp);
        
    //     char* cstyleinfo = ::ctime(&cstyle_t);
    //     std::cout << "[" << cstyleinfo << "]" << std::endl;

    //     std::string data = cstyleinfo;
    //     std::regex dtimeregex{ R"(^(\w{3}) (\w{3}) (\d{2}) (\d{2}):(\d{2}):(\d{2}) (\d{4})$)" };
    //     std::smatch match;

    //     // Sat May 25 17:12:51 2024
    //     if (std::regex_search(data, match, dtimeregex)) {
    //         // Match the group and subgroups by regex parser.
    //         std::cout << "matches for '" << data << "'\n";
    //         std::cout << "Prefix: '" << match.prefix() << "'\n";
    //         for (std::size_t i = 0; i < match.size(); ++i) 
    //             std::cout << i << ": " << match[i] << '\n';
    //         std::cout << "Suffix: '" << match.suffix() << "\'\n\n";
    //     }

    //     return DateTime(cstyleinfo);
    // }

    std::string year_month_day_h() {
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(time_point_)};
        std::stringstream ss;
        ss << ymd;
        return ss.str();
    }

    void currentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::cout << "Current time: " << std::ctime(&now_time) << std::endl;
    }

    std::string toString() const {
        // auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        // return std::format("{:%Y-%m-%d %H:%M:%S}", *std::localtime(&in_time_t));

        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void addSeconds(int seconds) {
        time_point_ += std::chrono::seconds(seconds);
    }

    void subtractSeconds(int seconds) {
        time_point_ -= std::chrono::seconds(seconds);
    }

    void addDays(int days) {
        time_point_ += std::chrono::hours(days * 24);
    }

    void subtractDays(int days) {
        time_point_ -= std::chrono::hours(days * 24);
    }

    void addMonths(int months) {
        std::tm tm = toTm();
        tm.tm_mon += months;
        normalizeTm(tm);
        updateFromTm(tm);
    }

    void subtractMonths(int months) {
        addMonths(-months);
    }

    void addYears(int years) {
        std::tm tm = toTm();
        tm.tm_year += years;
        normalizeTm(tm);
        updateFromTm(tm);
    }

    void subtractYears(int years) {
        addYears(-years);
    }

private:
    std::tm toTm() const {
        auto in_time_t = std::chrono::system_clock::to_time_t(time_point_);
        std::tm tm = *std::localtime(&in_time_t);
        return tm;
    }

    void normalizeTm(std::tm& tm) const {
        // Normalize the time structure (e.g., handle overflow of months into years)
        std::mktime(&tm);
    }

    void updateFromTm(const std::tm& tm) {
        // Convert the normalized tm back to time_point_
        std::time_t time = std::mktime(const_cast<std::tm*>(&tm));
        if (time == -1) {
            throw std::invalid_argument("Invalid date after normalization");
        }
        time_point_ = std::chrono::system_clock::from_time_t(time);
    }

private:
    std::chrono::system_clock::time_point time_point_;
};

} // namespace eden

