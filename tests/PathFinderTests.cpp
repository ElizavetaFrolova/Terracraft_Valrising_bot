#include <gtest/gtest.h>

#include <vector>

#include "algorithms/PathFinder.hpp"
#include "bot/BotKnowledge.hpp"
#include "bot/KnownDungeonView.hpp"
#include "model/Dungeon.hpp"
#include "model/Room.hpp"

namespace {

Dungeon createTestDungeon() {
    Dungeon dungeon;
    dungeon.setRoomCount(5);

    std::vector<Room> rooms(6);

    rooms[0] = Room(0, {1, 2}, {0, 0, 0, 0});
    rooms[1] = Room(1, {0, 3}, {0, 0, 0, 0});
    rooms[2] = Room(2, {0, 4}, {0, 0, 0, 0});
    rooms[3] = Room(3, {1, 4}, {0, 0, 0, 0});
    rooms[4] = Room(4, {2, 3, 5}, {0, 0, 0, 0});
    rooms[5] = Room(5, {4}, {0, 0, 0, 0});

    dungeon.setRooms(rooms);

    return dungeon;
}

Dungeon createTieBreakerDungeon() {
    Dungeon dungeon;
    dungeon.setRoomCount(100);

    std::vector<Room> rooms(101);

    rooms[0] = Room(0, {1, 2}, {0, 0, 0, 0});
    rooms[1] = Room(1, {0, 100}, {0, 0, 0, 0});
    rooms[2] = Room(2, {0, 3}, {0, 0, 0, 0});
    rooms[3] = Room(3, {2}, {0, 0, 0, 0});
    rooms[100] = Room(100, {1}, {0, 0, 0, 0});

    for (int i = 4; i < 100; ++i) {
        rooms[i] = Room(i, {}, {0, 0, 0, 0});
    }

    dungeon.setRooms(rooms);

    return dungeon;
}

void revealAfterVisit(const Dungeon& dungeon, BotKnowledge& knowledge, int roomId) {
    
    knowledge.markVisited(roomId);

    const Room& visitedRoom = dungeon.getRoom(roomId);

    for (int visibleRoomId : visitedRoom.getNeighbors()) {
        knowledge.markVisible(visibleRoomId);

        const Room& visibleRoom = dungeon.getRoom(visibleRoomId);

        for (int knownRoomId : visibleRoom.getNeighbors()) {
            knowledge.markNumberKnown(knownRoomId);
        }
    }
}

}

TEST(PathFinderTests, FindsNearestUnvisitedRoom) {
    Dungeon dungeon = createTestDungeon();

    BotKnowledge knowledge(5);

    revealAfterVisit(dungeon, knowledge, 0);
    revealAfterVisit(dungeon, knowledge, 1);
    revealAfterVisit(dungeon, knowledge, 3);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findPathToNearestUnvisitedRoom(view, 3);

    std::vector<int> expected = {3, 4};

    EXPECT_EQ(path, expected);
}

TEST(PathFinderTests, ChoosesSmallestRoomIdAmongNearestUnvisitedRooms) {
    Dungeon dungeon = createTieBreakerDungeon();

    BotKnowledge knowledge(100);

    revealAfterVisit(dungeon, knowledge, 0);
    revealAfterVisit(dungeon, knowledge, 1);
    revealAfterVisit(dungeon, knowledge, 2);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findPathToNearestUnvisitedRoom(view, 0);

    std::vector<int> expected = {0, 2, 3};

    EXPECT_EQ(path, expected);
}

TEST(PathFinderTests, ReturnsEmptyPathWhenAllRoomsVisited) {
    Dungeon dungeon = createTestDungeon();

    BotKnowledge knowledge(5);

    for (int roomId = 0; roomId <= 5; ++roomId) {
        revealAfterVisit(dungeon, knowledge, roomId);
    }

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findPathToNearestUnvisitedRoom(view, 3);

    EXPECT_TRUE(path.empty());
}

TEST(PathFinderTests, FindsShortestPathThroughVisitedRooms) {
    Dungeon dungeon = createTestDungeon();

    BotKnowledge knowledge(5);

    revealAfterVisit(dungeon, knowledge, 0);
    revealAfterVisit(dungeon, knowledge, 1);
    revealAfterVisit(dungeon, knowledge, 2);
    revealAfterVisit(dungeon, knowledge, 3);
    revealAfterVisit(dungeon, knowledge, 4);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findShortestPathThroughVisitedRooms(view, 4, 0);

    std::vector<int> expected = {4, 2, 0};

    EXPECT_EQ(path, expected);
}

TEST(PathFinderTests, FindsSingleRoomPathThroughVisitedRoomsWhenStartEqualsFinish) {
    Dungeon dungeon = createTestDungeon();

    BotKnowledge knowledge(5);

    revealAfterVisit(dungeon, knowledge, 3);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findShortestPathThroughVisitedRooms(view, 3, 3);

    std::vector<int> expected = {3};

    EXPECT_EQ(path, expected);
}

TEST(PathFinderTests, DoesNotUseUnvisitedRoomsInReturnPath) {
    Dungeon dungeon = createTestDungeon();

    BotKnowledge knowledge(5);

    revealAfterVisit(dungeon, knowledge, 0);
    revealAfterVisit(dungeon, knowledge, 1);
    revealAfterVisit(dungeon, knowledge, 3);
    revealAfterVisit(dungeon, knowledge, 4);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findShortestPathThroughVisitedRooms(view, 4, 0);

    std::vector<int> expected = {4, 3, 1, 0};

    EXPECT_EQ(path, expected);
}

TEST(PathFinderTests, ReturnsEmptyPathWhenFinishIsUnreachableThroughVisitedRooms) {
    Dungeon dungeon = createTestDungeon();

    BotKnowledge knowledge(5);

    revealAfterVisit(dungeon, knowledge, 0);
    revealAfterVisit(dungeon, knowledge, 5);

    KnownDungeonView view(dungeon, knowledge);

    std::vector<int> path =
        PathFinder::findShortestPathThroughVisitedRooms(view, 5, 0);

    EXPECT_TRUE(path.empty());
}