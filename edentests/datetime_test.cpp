
#include <gtest/gtest.h>
#include "datetime.h"

// Demonstrate some basic assertions.
TEST(DateTimeTest, BasicAssertions) {
    try {
        eden::DateTime dt;
        std::cout << "Current DateTime: " << dt.toString() << std::endl;

        dt.addSeconds(3600); // Add one hour
        std::cout << "After adding one hour: " << dt.toString() << std::endl;

        dt.subtractSeconds(7200); // Subtract two hours
        std::cout << "After subtracting two hours: " << dt.toString() << std::endl;

        // Compare two DateTime objects
        eden::DateTime dt1(2023, 5, 25);
        eden::DateTime dt2(2023, 5, 25);
        EXPECT_EQ(dt1, dt2);

        // Create DateTime object with year, month, and day
        dt = eden::DateTime(2023, 5, 25);
        std::cout << "Specific DateTime: " << dt.toString() << std::endl;

        dt.addDays(1); // Add one day
        std::cout << "After adding one day: " << dt.toString() << std::endl;
        EXPECT_EQ("2023-05-26", dt.year_month_day_h());
        EXPECT_EQ("20230526", dt.year_month_day());
        EXPECT_EQ("202305", dt.year_month());
        EXPECT_EQ("2023-05", dt.year_month_h());

        dt.subtractDays(2); // Subtract two days
        std::cout << "After subtracting two days: " << dt.toString() << std::endl;
        EXPECT_EQ("2023-05-24", dt.year_month_day_h());
        EXPECT_EQ("20230524", dt.year_month_day());
        EXPECT_EQ("202305", dt.year_month());
        EXPECT_EQ("2023-05", dt.year_month_h());
        EXPECT_EQ("2023", dt.year());

        dt.addMonths(1); // Add one month
        std::cout << "After adding one month: " << dt.toString() << std::endl;
        EXPECT_EQ("2023-06-24", dt.year_month_day_h());
        EXPECT_EQ("20230624", dt.year_month_day());
        EXPECT_EQ("202306", dt.year_month());
        EXPECT_EQ("2023-06", dt.year_month_h());

        dt.subtractMonths(2); // Subtract two months
        std::cout << "After subtracting two months: " << dt.toString() << std::endl;
        EXPECT_EQ("2023-04-24", dt.year_month_day_h());
        EXPECT_EQ("20230424", dt.year_month_day());
        EXPECT_EQ("202304", dt.year_month());
        EXPECT_EQ("2023-04", dt.year_month_h());

        dt.addYears(1); // Add one year
        std::cout << "After adding one year: " << dt.toString() << std::endl;
        EXPECT_EQ("2024-04-24", dt.year_month_day_h());
        EXPECT_EQ("20240424", dt.year_month_day());
        EXPECT_EQ("202404", dt.year_month());
        EXPECT_EQ("2024-04", dt.year_month_h());
        EXPECT_EQ("2024", dt.year());

        dt.subtractYears(2); // Subtract two years
        std::cout << "After subtracting two years: " << dt.toString() << std::endl;
        EXPECT_EQ("2022-04-24", dt.year_month_day_h());
        EXPECT_EQ("20220424", dt.year_month_day());
        EXPECT_EQ("202204", dt.year_month());
        EXPECT_EQ("2022-04", dt.year_month_h());

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

}