#include <gtest/gtest.h>

#include <vector>

#include "bot/BotKnowledge.hpp"

TEST(BotKnowledgeTests, ConstructorMarksAllRoomsAsUnknown) {
    BotKnowledge knowledge(4);

    for (int roomId = 0; roomId <= 4; ++roomId) {
        EXPECT_EQ(knowledge.getRoomLevel(roomId), RoomKnowledgeLevel::Unknown);
        EXPECT_TRUE(knowledge.isUnknown(roomId));
        EXPECT_FALSE(knowledge.isNumberKnown(roomId));
        EXPECT_FALSE(knowledge.isVisible(roomId));
        EXPECT_FALSE(knowledge.isVisited(roomId));
    }
}

TEST(BotKnowledgeTests, MarkVisitedMarksRoomAsVisited) {
    BotKnowledge knowledge(4);

    knowledge.markVisited(0);

    EXPECT_TRUE(knowledge.isVisited(0));
    EXPECT_EQ(knowledge.getRoomLevel(0), RoomKnowledgeLevel::Visited);
}

TEST(BotKnowledgeTests, MarkVisibleMarksRoomAsVisible) {
    BotKnowledge knowledge(4);

    knowledge.markVisible(1);

    EXPECT_TRUE(knowledge.isVisible(1));
    EXPECT_EQ(knowledge.getRoomLevel(1), RoomKnowledgeLevel::Visible);
}

TEST(BotKnowledgeTests, MarkNumberKnownMarksRoomAsNumberKnown) {
    BotKnowledge knowledge(4);

    knowledge.markNumberKnown(3);

    EXPECT_TRUE(knowledge.isNumberKnown(3));
    EXPECT_EQ(knowledge.getRoomLevel(3), RoomKnowledgeLevel::NumberKnown);
}

TEST(BotKnowledgeTests, MarkVisibleUpgradesNumberKnownRoom) {
    BotKnowledge knowledge(4);

    knowledge.markNumberKnown(3);
    EXPECT_TRUE(knowledge.isNumberKnown(3));

    knowledge.markVisible(3);
    EXPECT_TRUE(knowledge.isVisible(3));
}

TEST(BotKnowledgeTests, MarkVisitedUpgradesVisibleRoom) {
    BotKnowledge knowledge(4);

    knowledge.markVisible(2);
    EXPECT_TRUE(knowledge.isVisible(2));

    knowledge.markVisited(2);
    EXPECT_TRUE(knowledge.isVisited(2));
}

TEST(BotKnowledgeTests, LowerKnowledgeLevelDoesNotDowngradeRoom) {
    BotKnowledge knowledge(4);

    knowledge.markVisited(1);

    knowledge.markVisible(1);
    knowledge.markNumberKnown(1);

    EXPECT_TRUE(knowledge.isVisited(1));
    EXPECT_EQ(knowledge.getRoomLevel(1), RoomKnowledgeLevel::Visited);
}

TEST(BotKnowledgeTests, CanSeeRoomNumberReturnsTrueForNumberKnownVisibleAndVisitedRooms) {
    BotKnowledge knowledge(4);

    knowledge.markNumberKnown(1);
    knowledge.markVisible(2);
    knowledge.markVisited(3);

    EXPECT_FALSE(knowledge.canSeeRoomNumber(0));
    EXPECT_TRUE(knowledge.canSeeRoomNumber(1));
    EXPECT_TRUE(knowledge.canSeeRoomNumber(2));
    EXPECT_TRUE(knowledge.canSeeRoomNumber(3));
}

TEST(BotKnowledgeTests, CanSeeNeighborsOnlyForVisibleAndVisitedRooms) {
    BotKnowledge knowledge(4);

    knowledge.markNumberKnown(1);
    knowledge.markVisible(2);
    knowledge.markVisited(3);

    EXPECT_FALSE(knowledge.canSeeNeighbors(0));
    EXPECT_FALSE(knowledge.canSeeNeighbors(1));
    EXPECT_TRUE(knowledge.canSeeNeighbors(2));
    EXPECT_TRUE(knowledge.canSeeNeighbors(3));
}

TEST(BotKnowledgeTests, CanSeeResourcesOnlyForVisitedRooms) {
    BotKnowledge knowledge(4);

    knowledge.markNumberKnown(1);
    knowledge.markVisible(2);
    knowledge.markVisited(3);

    EXPECT_FALSE(knowledge.canSeeResources(0));
    EXPECT_FALSE(knowledge.canSeeResources(1));
    EXPECT_FALSE(knowledge.canSeeResources(2));
    EXPECT_TRUE(knowledge.canSeeResources(3));
}

TEST(BotKnowledgeTests, GetVisitedRoomsReturnsCorrectFlags) {
    BotKnowledge knowledge(4);

    knowledge.markVisited(0);
    knowledge.markVisited(1);

    std::vector<bool> visitedRooms = knowledge.getVisitedRooms();

    ASSERT_EQ(visitedRooms.size(), 5);

    EXPECT_TRUE(visitedRooms[0]);
    EXPECT_TRUE(visitedRooms[1]);
    EXPECT_FALSE(visitedRooms[2]);
    EXPECT_FALSE(visitedRooms[3]);
    EXPECT_FALSE(visitedRooms[4]);
}