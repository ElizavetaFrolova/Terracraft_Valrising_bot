#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "bot/AliceStrategy.hpp"
#include "bot/Bot.hpp"
#include "bot/BotController.hpp"
#include "io/Parser.hpp"
#include "model/Dungeon.hpp"
#include "model/Resource.hpp"
#include "model/Room.hpp"

namespace {

Dungeon createExampleDungeon() {
    Dungeon dungeon;

    dungeon.setRoomCount(5);
    dungeon.setFood(6);
    dungeon.setTargetResource(Resource::Gems);

    std::vector<Room> rooms(6);

    rooms[0] = Room(0, {1, 2}, {0, 0, 0, 0});
    rooms[1] = Room(1, {0, 3}, {5, 2, 1, 15});
    rooms[2] = Room(2, {0, 4}, {3, 2, 1, 10});
    rooms[3] = Room(3, {1, 4}, {1, 0, 2, 40});
    rooms[4] = Room(4, {2, 3, 5}, {2, 4, 0, 15});
    rooms[5] = Room(5, {4}, {0, 5, 4, 10});

    dungeon.setRooms(rooms);

    return dungeon;
}

Dungeon createReturnCollectionDungeon() {
    Dungeon dungeon;

    dungeon.setRoomCount(1);
    dungeon.setFood(4);
    dungeon.setTargetResource(Resource::Iron);

    std::vector<Room> rooms(2);

    rooms[0] = Room(0, {1}, {0, 0, 0, 0});
    rooms[1] = Room(1, {0}, {1, 1, 0, 0});

    dungeon.setRooms(rooms);

    return dungeon;
}

std::string runAliceStrategy(Dungeon& dungeon) {
    std::ostringstream output;

    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    BotController controller(dungeon, bot, output);

    AliceStrategy strategy;
    strategy.run(controller);

    return output.str();
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

void removeFile(const std::string& filename) {
    std::remove(filename.c_str());
}

} // namespace

TEST(AliceStrategyTests, RunsExampleScenarioCorrectly) {
    Dungeon dungeon = createExampleDungeon();

    const std::string output = runAliceStrategy(dungeon);

    const std::string expected =
        "go 1\n"
        "state 1 5 2 1 15\n"
        "collect gems\n"
        "state 1 5 2 _ 15\n"
        "go 3\n"
        "state 3 1 0 2 40\n"
        "collect gems\n"
        "state 3 1 0 _ 40\n"
        "go 4\n"
        "state 4 2 4 0 15\n"
        "collect gold\n"
        "state 4 2 _ 0 15\n"
        "go 3\n"
        "state 3 1 0 _ 40\n"
        "go 1\n"
        "state 1 5 2 _ 15\n"
        "go 0\n"
        "result 0 4 3 0 182\n";

    EXPECT_EQ(output, expected);
}

TEST(AliceStrategyTests, DoesNotPrintStateAfterReturningToStartRoom) {
    Dungeon dungeon = createExampleDungeon();

    const std::string output = runAliceStrategy(dungeon);

    EXPECT_NE(output.find("go 0\nresult"), std::string::npos);
    EXPECT_EQ(output.find("go 0\nstate 0"), std::string::npos);
}

TEST(AliceStrategyTests, CollectsExtraResourcesOnReturnWhenFoodIsEnough) {
    Dungeon dungeon = createReturnCollectionDungeon();

    const std::string output = runAliceStrategy(dungeon);

    const std::string expected =
        "go 1\n"
        "state 1 1 1 0 0\n"
        "collect iron\n"
        "state 1 _ 1 0 0\n"
        "collect gold\n"
        "state 1 _ _ 0 0\n"
        "go 0\n"
        "result 1 1 0 0 25\n";

    EXPECT_EQ(output, expected);
}

TEST(AliceStrategyTests, RunsExampleScenarioParsedFromFile) {
    const std::string filename = "alice_strategy_example_input.txt";

    writeFile(
        filename,
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2,5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n"
    );

    ParseResult parseResult = Parser::parseFile(filename);

    ASSERT_TRUE(parseResult.success);

    const std::string output = runAliceStrategy(parseResult.dungeon);

    const std::string expected =
        "go 1\n"
        "state 1 5 2 1 15\n"
        "collect gems\n"
        "state 1 5 2 _ 15\n"
        "go 3\n"
        "state 3 1 0 2 40\n"
        "collect gems\n"
        "state 3 1 0 _ 40\n"
        "go 4\n"
        "state 4 2 4 0 15\n"
        "collect gold\n"
        "state 4 2 _ 0 15\n"
        "go 3\n"
        "state 3 1 0 _ 40\n"
        "go 1\n"
        "state 1 5 2 _ 15\n"
        "go 0\n"
        "result 0 4 3 0 182\n";

    EXPECT_EQ(output, expected);

    removeFile(filename);
}