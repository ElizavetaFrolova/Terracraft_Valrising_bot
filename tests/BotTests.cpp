#include <gtest/gtest.h>

#include "bot/Bot.hpp"

TEST(BotTests, ConstructorInitializesState) {
    Bot bot(6, 5);

    EXPECT_EQ(bot.getCurrentRoom(), 0);
    EXPECT_EQ(bot.getFood(), 6);
    EXPECT_EQ(bot.getTotalValue(), 0);

    const auto& collected = bot.getCollectedResources();

    EXPECT_EQ(collected[resourceIndex(Resource::Iron)], 0);
    EXPECT_EQ(collected[resourceIndex(Resource::Gold)], 0);
    EXPECT_EQ(collected[resourceIndex(Resource::Gems)], 0);
    EXPECT_EQ(collected[resourceIndex(Resource::Exp)], 0);

    EXPECT_FALSE(bot.isResourceCollectedInRoom(0, Resource::Iron));
    EXPECT_FALSE(bot.isResourceCollectedInRoom(0, Resource::Gold));
    EXPECT_FALSE(bot.isResourceCollectedInRoom(0, Resource::Gems));
    EXPECT_FALSE(bot.isResourceCollectedInRoom(0, Resource::Exp));
}

TEST(BotTests, MoveToChangesCurrentRoomAndSpendsFood) {
    Bot bot(6, 5);

    bot.moveTo(1);

    EXPECT_EQ(bot.getCurrentRoom(), 1);
    EXPECT_EQ(bot.getFood(), 5);
}

TEST(BotTests, SpendFoodDecreasesFood) {
    Bot bot(6, 5);

    bot.spendFood();
    EXPECT_EQ(bot.getFood(), 5);

    bot.spendFood(3);
    EXPECT_EQ(bot.getFood(), 2);
}

TEST(BotTests, CanSpendFoodWhenEnoughFood) {
    Bot bot(5, 5);

    EXPECT_TRUE(bot.canSpendFood());
    EXPECT_TRUE(bot.canSpendFood(1));
    EXPECT_TRUE(bot.canSpendFood(5));
}

TEST(BotTests, CannotSpendFoodWhenNotEnoughFood) {
    Bot bot(2, 5);

    EXPECT_FALSE(bot.canSpendFood(3));
}

TEST(BotTests, CannotSpendFoodWhenFoodIsZero) {
    Bot bot(0, 5);

    EXPECT_FALSE(bot.canSpendFood());
}

TEST(BotTests, CollectResourceUpdatesCollectedResourcesAndTotalValue) {
    Bot bot(6, 5);

    bot.collectResource(Resource::Gems, 2, 46);

    const auto& collected = bot.getCollectedResources();

    EXPECT_EQ(collected[resourceIndex(Resource::Iron)], 0);
    EXPECT_EQ(collected[resourceIndex(Resource::Gold)], 0);
    EXPECT_EQ(collected[resourceIndex(Resource::Gems)], 2);
    EXPECT_EQ(collected[resourceIndex(Resource::Exp)], 0);

    EXPECT_EQ(bot.getTotalValue(), 92);
}

TEST(BotTests, CollectResourceMarksResourceCollectedInCurrentRoom) {
    Bot bot(6, 5);

    bot.moveTo(1);
    bot.collectResource(Resource::Gems, 2, 46);

    EXPECT_TRUE(bot.isResourceCollectedInRoom(1, Resource::Gems));
    EXPECT_FALSE(bot.isResourceCollectedInRoom(1, Resource::Iron));
    EXPECT_FALSE(bot.isResourceCollectedInRoom(0, Resource::Gems));
}

TEST(BotTests, GetCollectedInRoomReturnsCorrectArray) {
    Bot bot(6, 5);

    bot.moveTo(2);
    bot.collectResource(Resource::Iron, 3, 7);
    bot.collectResource(Resource::Exp, 10, 1);

    const auto& collectedInRoom = bot.getCollectedInRoom(2);

    EXPECT_TRUE(collectedInRoom[resourceIndex(Resource::Iron)]);
    EXPECT_FALSE(collectedInRoom[resourceIndex(Resource::Gold)]);
    EXPECT_FALSE(collectedInRoom[resourceIndex(Resource::Gems)]);
    EXPECT_TRUE(collectedInRoom[resourceIndex(Resource::Exp)]);
}