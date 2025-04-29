#pragma once

#include <chrono>
#include <format>  // for C++20
#include <iostream>

using namespace std::literals; 

namespace eden {

/**
 * @brief TimeElapsed class
 * @details This class is used to measure elapsed time.
 * It provides methods to start the timer, measure elapsed time, and print it.
 */
struct sTimeElapsed {
    std::chrono::time_point<std::chrono::steady_clock> start;

    void timeStart() {        
        start = std::chrono::steady_clock::now();
    }

    template<typename T>
    void getElapsed() const {
        const auto end = std::chrono::steady_clock::now();
        auto value = std::chrono::duration_cast<T>(end - start);
        std::cout << "Elapsed: " << value.count() << " " << typeid(T).name() << std::endl;
    }

    void msElapsed() const {
        const auto end = std::chrono::steady_clock::now();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Elapsed: " << value << " ms" << std::endl;
    }

    void nsElapsed() const {
        const auto end = std::chrono::steady_clock::now();
        // auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Elapsed: " << (end - start) / 1ns << "ns." <<  std::endl;
    }

    void secElapsed() const {
        const auto end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> elapsed_seconds(end - start);
        std::cout << "Elapsed: ";
        // std::cout << elapsed_seconds.count() << '\n';  // before C++20
        std::cout << elapsed_seconds << std::endl;  // C++20
    }

    static std::string yearMonthDayHyphen() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
 
        // std::cout << "Current Year: " << static_cast<int>(ymd.year()) << ", "
        //             "Month: " << static_cast<unsigned>(ymd.month()) << ", "
        //             "Day: " << static_cast<unsigned>(ymd.day()) << "\n"
        //             "ymd: " << ymd << '\n';

        std::stringstream ss;
        ss << ymd;
        return ss.str();
    }

    static std::string year() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
        
        std::stringstream ss;
        ss << ymd.year();
        
        return ss.str();
    }

    static std::string yearMonth() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};
        
        std::stringstream ss;
        // ss << ymd.year() << std::format("{%m}", static_cast<unsigned>(ymd.month()));
        ss << ymd.year() << static_cast<unsigned>(ymd.month());
        
        return ss.str();
    }

    static std::string yearMonthDay() {
        const std::chrono::time_point now{std::chrono::system_clock::now()};
        const std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(now)};

        // return std::format(
        //     "{}{}{}", 
        //     static_cast<int>(ymd.year()),
        //     static_cast<int>(ymd.month()),
        //     static_cast<int>(ymd.day()));
        
        std::stringstream ss;
        ss << ymd.year() << static_cast<unsigned>(ymd.month()) << ymd.day();
        return ss.str();
    }
};

} // namespace eden

