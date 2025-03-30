
#include <gtest/gtest.h>
#include "core.h"

TEST(ResponseTest, BasicAssertions) {

    constexpr auto resp = eden::Response(eden::Status::ERROR, "An error occurred"); 

    EXPECT_EQ(resp.status, eden::Status::ERROR);
    EXPECT_EQ(resp.msg, "An error occurred");
}