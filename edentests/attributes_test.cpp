
#include <gtest/gtest.h>
#include "attributes.h"

TEST(AttributesTest, DefaultConstructorDeleted) {
    // Type trait in C++ that checks if a class can be default constructed
    // meaning that it has a default constructor that can be called with no arguments
    EXPECT_TRUE(std::is_default_constructible<eden::Attributes>::value == false);
}

TEST(AttributesTest, ConstructorAcceptingCobAsReference) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    EXPECT_EQ(attributes.cob(), cob);
}

TEST(AttributesTest, CopyConstructor) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    eden::Attributes copy(attributes);
    EXPECT_EQ(copy.cob(), cob);
}

TEST(AttributesTest, MoveConstructor) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    eden::Attributes move(std::move(attributes));
    EXPECT_EQ(move.cob(), cob);
}

TEST(AttributesTest, AssignmentOperator) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    eden::Attributes copy = attributes;
    EXPECT_EQ(copy.cob(), cob);
}

TEST(AttributesTest, MoveAssignmentOperator) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    eden::Attributes move = std::move(attributes);
    EXPECT_EQ(move.cob(), cob);
}

TEST(AttributesTest, EqualityOperator) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    eden::Attributes copy(attributes);
    EXPECT_TRUE(attributes == copy);
}

TEST(AttributesTest, InequalityOperator) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    eden::DateTime cob2(2025, 1, 2);
    eden::Attributes copy(cob2);
    EXPECT_TRUE(attributes != copy);
}

TEST(AttributesTest, GetterForCob) {
    eden::DateTime cob(2025, 1, 1);
    eden::Attributes attributes(cob);
    EXPECT_EQ(attributes.cob(), cob);
}

