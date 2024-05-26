
#include <gtest/gtest.h>
#include "datetime.h"

// Demonstrate some basic assertions.
TEST(DateTimeTest, BasicAssertions) {
    eden::DateTime dt;
    std::cout << "Current DateTime: " << dt.toString() << std::endl;

    dt.addSeconds(3600); // Add one hour
    std::cout << "After adding one hour: " << dt.toString() << std::endl;

    dt.subtractSeconds(7200); // Subtract two hours
    std::cout << "After subtracting two hours: " << dt.toString() << std::endl;

    try {
        // Default constructor, current date and time
        eden::DateTime dtNow;
        std::cout << "Current DateTime: " << dtNow.toString() << std::endl;

        // Create DateTime object with year, month, and day
        eden::DateTime dt(2023, 5, 25);
        std::cout << "Specific DateTime: " << dt.toString() << std::endl;

        dt.addDays(1); // Add one day
        std::cout << "After adding one day: " << dt.toString() << std::endl;

        dt.subtractDays(2); // Subtract two days
        std::cout << "After subtracting two days: " << dt.toString() << std::endl;

        // Compare two DateTime objects
        eden::DateTime dt1(2023, 5, 25);
        eden::DateTime dt2(2023, 5, 25);
        if (dt1 == dt2) {
            std::cout << "dt1 and dt2 are equal" << std::endl;
        } else {
            std::cout << "dt1 and dt2 are not equal" << std::endl;
        }

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}