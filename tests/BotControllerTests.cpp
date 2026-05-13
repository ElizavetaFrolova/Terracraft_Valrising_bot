#include <gtest/gtest.h>

#include <array>
#include <sstream>
#include <vector>

#include "bot/Bot.hpp"
#include "bot/BotController.hpp"
#include "model/Dungeon.hpp"
#include "model/Resource.hpp"
#include "model/Room.hpp"

namespace {

Dungeon createSimpleDungeon() {
    Dungeon dungeon;

    dungeon.setRoomCount(3);
    dungeon.setFood(6);
    dungeon.setTargetResource(Resource::Gems);

    std::vector<Room> rooms;

    rooms.emplace_back(0, std::vector<int>{1, 2}
        , std::array<int, RESOURCE_COUNT>{0, 0, 0, 0});

    rooms.emplace_back(1, std::vector<int>{0, 3}
        , std::array<int, RESOURCE_COUNT>{5, 2, 1, 15});

    rooms.emplace_back(2, std::vector<int>{0}
        , std::array<int, RESOURCE_COUNT>{3, 0, 0, 10}
    );

    rooms.emplace_back(3, std::vector<int>{1}
        , std::array<int, RESOURCE_COUNT>{0, 4, 2, 0}
    );

    dungeon.setRooms(rooms);

    return dungeon;
}

}

TEST(BotControllerTests, ConstructorRevealsStartRoomKnowledge) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    const BotKnowledge& knowledge = controller.getKnowledge();

    EXPECT_TRUE(knowledge.isVisited(0));
    EXPECT_TRUE(knowledge.isVisible(1));
    EXPECT_TRUE(knowledge.isVisible(2));
    EXPECT_TRUE(knowledge.canSeeRoomNumber(3));
}

TEST(BotControllerTests, GetCurrentRoomAndFoodReturnsBotState) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    EXPECT_EQ(controller.getCurrentRoom(), 0);
    EXPECT_EQ(controller.getFood(), 6);
    EXPECT_EQ(controller.getInitialFood(), 6);
}

TEST(BotControllerTests, GetKnownNeighborsReturnsNeighborsForVisibleRoom) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    std::vector<int> expected{0, 3};

    EXPECT_EQ(controller.getKnownNeighbors(1), expected);
}

TEST(BotControllerTests, MoveToSpendsFoodChangesRoomPrintsGoAndRevealsRoom) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    bool moved = controller.moveTo(1);

    EXPECT_TRUE(moved);
    EXPECT_EQ(controller.getCurrentRoom(), 1);
    EXPECT_EQ(controller.getFood(), 5);

    EXPECT_EQ(out.str(), "go 1\n");

    const BotKnowledge& knowledge = controller.getKnowledge();

    EXPECT_TRUE(knowledge.isVisited(1));
    EXPECT_TRUE(knowledge.isVisible(3));
}

TEST(BotControllerTests, CollectExistingResourceAddsResourceAndPrintsCollect) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);

    bool collected = controller.collect(Resource::Gems);

    EXPECT_TRUE(collected);

    const auto& collectedResources = bot.getCollectedResources();

    EXPECT_EQ(collectedResources[resourceIndex(Resource::Gems)], 1);

    EXPECT_EQ(bot.getTotalValue(), 46);

    EXPECT_EQ(out.str(), "go 1\ncollect gems\n");
}

TEST(BotControllerTests, FirstCollectionInRoomDoesNotSpendFood) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);

    int foodBeforeCollect = controller.getFood();

    bool collected = controller.collect(Resource::Gems);

    EXPECT_TRUE(collected);
    EXPECT_EQ(controller.getFood(), foodBeforeCollect);
}

TEST(BotControllerTests, SecondCollectionInSameRoomSpendsFood) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);

    bool firstCollected = controller.collect(Resource::Gems);
    int foodAfterFirstCollect = controller.getFood();

    bool secondCollected = controller.collect(Resource::Gold);

    EXPECT_TRUE(firstCollected);
    EXPECT_TRUE(secondCollected);

    EXPECT_EQ(controller.getFood(), foodAfterFirstCollect - 1);
}

TEST(BotControllerTests, CollectMissingResourceReturnsFalseAndDoesNotPrintCollect) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(2);

    bool collected = controller.collect(Resource::Gold);

    EXPECT_FALSE(collected);
    EXPECT_EQ(out.str(), "go 2\n");

    const auto& collectedResources = bot.getCollectedResources();

    EXPECT_EQ(collectedResources[resourceIndex(Resource::Gold)], 0);
}

TEST(BotControllerTests, PrintStateShowsUnderscoreForCollectedResource) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);
    controller.collect(Resource::Gems);
    controller.printState();

    EXPECT_EQ(
        out.str(),
        "go 1\n"
        "collect gems\n"
        "state 1 5 2 _ 15\n"
    );
}

TEST(BotControllerTests, PrintResultShowsCollectedResourcesAndTotalValue) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);
    controller.collect(Resource::Gems);
    controller.printResult();

    EXPECT_EQ(
        out.str(),
        "go 1\n"
        "collect gems\n"
        "result 0 0 1 0 46\n"
    );
}

TEST(BotControllerTests, GetCurrentRoomResourcesReturnsResourcesOfCurrentRoom) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);

    std::array<int, RESOURCE_COUNT> expected{5, 2, 1, 15};

    EXPECT_EQ(controller.getCurrentRoomResources(), expected);
}

TEST(BotControllerTests, FindReturnPathToStartReturnsPathThroughVisitedRooms) {
    Dungeon dungeon = createSimpleDungeon();
    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    std::ostringstream out;

    BotController controller(dungeon, bot, out);

    controller.moveTo(1);
    controller.moveTo(3);

    std::vector<int> expected{3, 1, 0};

    EXPECT_EQ(controller.findReturnPathToStart(), expected);
}