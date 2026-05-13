#include <gtest/gtest.h>

#include <array>
#include <vector>

#include "bot/KnownDungeonView.hpp"
#include "bot/BotKnowledge.hpp"
#include "model/Dungeon.hpp"
#include "model/Room.hpp"
#include "model/Resource.hpp"

namespace {

Dungeon createSimpleDungeon() {
    Dungeon dungeon;
    dungeon.setRoomCount(3);

    std::vector<Room> rooms;

    rooms.emplace_back(0, std::vector<int>{1, 2}
        , std::array<int, RESOURCE_COUNT>{0, 0, 0, 0});

    rooms.emplace_back(1, std::vector<int>{0, 3}
        , std::array<int, RESOURCE_COUNT>{5, 0, 0, 0});

    rooms.emplace_back(2, std::vector<int>{0}
        , std::array<int, RESOURCE_COUNT>{0, 2, 0, 0});

    rooms.emplace_back(3, std::vector<int>{1}
        , std::array<int, RESOURCE_COUNT>{0, 0, 3, 0});

    dungeon.setRooms(rooms);

    return dungeon;
}

}

TEST(KnownDungeonViewTests, ReturnsRoomCountFromDungeon) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_EQ(view.getRoomCount(), 3);
}

TEST(KnownDungeonViewTests, IsVisitedReturnsTrueOnlyForVisitedRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markVisited(1);

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_TRUE(view.isVisited(1));
    EXPECT_FALSE(view.isVisited(0));
    EXPECT_FALSE(view.isVisited(2));
    EXPECT_FALSE(view.isVisited(3));
}

TEST(KnownDungeonViewTests, CanSeeNeighborsReturnsFalseForUnknownRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_FALSE(view.canSeeNeighbors(1));
}

TEST(KnownDungeonViewTests, CanSeeNeighborsReturnsFalseForNumberKnownRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markNumberKnown(1);

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_FALSE(view.canSeeNeighbors(1));
}

TEST(KnownDungeonViewTests, CanSeeNeighborsReturnsTrueForVisibleRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markVisible(1);

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_TRUE(view.canSeeNeighbors(1));
}

TEST(KnownDungeonViewTests, CanSeeNeighborsReturnsTrueForVisitedRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markVisited(1);

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_TRUE(view.canSeeNeighbors(1));
}

TEST(KnownDungeonViewTests, GetNeighborsReturnsEmptyVectorForUnknownRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_TRUE(view.getNeighbors(1).empty());
}

TEST(KnownDungeonViewTests, GetNeighborsReturnsEmptyVectorForNumberKnownRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markNumberKnown(1);

    KnownDungeonView view(dungeon, knowledge);

    EXPECT_TRUE(view.getNeighbors(1).empty());
}

TEST(KnownDungeonViewTests, GetNeighborsReturnsNeighborsForVisibleRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markVisible(1);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> expected{0, 3};

    EXPECT_EQ(view.getNeighbors(1), expected);
}

TEST(KnownDungeonViewTests, GetNeighborsReturnsNeighborsForVisitedRoom) {
    Dungeon dungeon = createSimpleDungeon();
    BotKnowledge knowledge(dungeon.getRoomCount());

    knowledge.markVisited(0);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> expected{1, 2};

    EXPECT_EQ(view.getNeighbors(0), expected);
}