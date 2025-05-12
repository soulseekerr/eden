
#include <gtest/gtest.h>
#include "dataframe.h"

TEST(DataFrameTest, BasicAssertions) {
    eden::DataFrame df;
    df.define_column("id");
    df.define_column("name");
    df.define_column("score");

    df.add_row({{"id", int32_t(1)}, {"name", "Alice"}, {"score", 85.5}});
    df.add_row({{"id", int32_t(2)}, {"name", "Bob"}, {"score", 92.0}});
    df.add_row({{"id", int32_t(3)}, {"name", "Charlie"}, {"score", 70.0}});

    std::cout << "--- All Rows ---\n";
    df.print_all();

    std::cout << "\n--- Filter: Score >= 90 ---\n";
    auto filtered = df.filter([&](size_t i) {
        const auto& score = df.get_column("score").at(i);
        return std::holds_alternative<double>(score) && std::get<double>(score) >= 90.0;
    });
    df.print_filtered(filtered);
}
