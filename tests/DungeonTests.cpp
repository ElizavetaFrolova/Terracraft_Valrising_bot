#include <gtest/gtest.h>

#include "model/Dungeon.hpp"

TEST(DungeonTests, TargetResourceDoublesValue) {
    Dungeon dungeon;

    dungeon.setTargetResource(Resource::Gems);

    const auto& values = dungeon.getResourceValues();

    EXPECT_EQ(values[resourceIndex(Resource::Iron)], 7);
    EXPECT_EQ(values[resourceIndex(Resource::Gold)], 11);
    EXPECT_EQ(values[resourceIndex(Resource::Gems)], 46);
    EXPECT_EQ(values[resourceIndex(Resource::Exp)], 1);
}

TEST(DungeonTests, FoodAndRoomCountSettersWork) {
    Dungeon dungeon;

    dungeon.setFood(6);
    dungeon.setRoomCount(5);

    EXPECT_EQ(dungeon.getFood(), 6);
    EXPECT_EQ(dungeon.getRoomCount(), 5);
}