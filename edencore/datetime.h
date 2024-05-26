#pragma once

#include <chrono>
#include <iostream>
#include <format>

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
    DateTime() {}

    DateTime(const char* cstyleinfo) {
        str_dt_ = cstyleinfo;
    }

    // Copy constructor
    DateTime(const DateTime& dt) {
        str_dt_ = dt.str_dt_;
    }

    // Move constructor
    DateTime(DateTime&& dt) noexcept {
        str_dt_ = dt.str_dt_;
        dt.str_dt_ = "";
    }

    virtual ~DateTime() {}

    // Assignment operator
    DateTime& operator=(const DateTime& dt) {
        if (this == &dt) {
            return *this;  // self-assignment
        }
        str_dt_ = dt.str_dt_;
        return *this;
    }

    // Move assignment operator
    DateTime& operator=(DateTime&& dt) {
        if (this == &dt) {
            return *this;  // self-assignment
        }
        str_dt_ = dt.str_dt_;
        return *this;
    }

    // Equality operator
    bool operator==(const DateTime& dt) {
        return (str_dt_ == dt.str_dt_);
    }

    static DateTime Now() {
        auto tp = std::chrono::system_clock::now();
        time_t cstyle_t = std::chrono::system_clock::to_time_t(tp);
        
        char* cstyleinfo = ::ctime(&cstyle_t);

        std::string data = cstyleinfo;
        std::regex dtimeregex{ R"(^(\w{3}) (\w{3}) (\d{2}) (\d{2}):(\d{2}):(\d{2}) (\d{4})$)" };
        std::smatch match;

        // Sat May 25 17:12:51 2024
        if (std::regex_search(data, match, dtimeregex)) {
            // Match the group and subgroups by regex parser.

            std::cout << "matches for '" << data << "'\n";
            std::cout << "Prefix: '" << match.prefix() << "'\n";
            for (std::size_t i = 0; i < match.size(); ++i) 
                std::cout << i << ": " << match[i] << '\n';
            std::cout << "Suffix: '" << match.suffix() << "\'\n\n";
        }

        return DateTime(cstyleinfo);
    }

    std::string year_month_day_h() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
 

        std::stringstream ss;
        ss << ymd;
        return str_dt_;
    }

private:
    std::string str_dt_ = "";
};

} // namespace eden

