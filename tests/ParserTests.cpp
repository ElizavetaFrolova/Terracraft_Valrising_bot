#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "io/Parser.hpp"

namespace {

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
}

void removeFile(const std::string& filename) {
    std::remove(filename.c_str());
}

} // namespace

TEST(ParserTests, ParsesValidInput) {
    const std::string filename = "valid_input.txt";

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

    ParseResult result = Parser::parseFile(filename);

    ASSERT_TRUE(result.success);

    const Dungeon& dungeon = result.dungeon;

    EXPECT_EQ(dungeon.getRoomCount(), 5);
    EXPECT_EQ(dungeon.getFood(), 6);
    EXPECT_EQ(dungeon.getTargetResource(), Resource::Gems);

    const auto& values = dungeon.getResourceValues();

    EXPECT_EQ(values[resourceIndex(Resource::Iron)], 7);
    EXPECT_EQ(values[resourceIndex(Resource::Gold)], 11);
    EXPECT_EQ(values[resourceIndex(Resource::Gems)], 46);
    EXPECT_EQ(values[resourceIndex(Resource::Exp)], 1);

    removeFile(filename);
}

TEST(ParserTests, ParsesRoomsCorrectly) {
    const std::string filename = "rooms_input.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gold\n"
    );

    ParseResult result = Parser::parseFile(filename);

    ASSERT_TRUE(result.success);

    const Dungeon& dungeon = result.dungeon;

    const Room& room1 = dungeon.getRoom(1);
    const auto& resources = room1.getResources();

    EXPECT_EQ(room1.getId(), 1);

    EXPECT_EQ(resources[resourceIndex(Resource::Iron)], 5);
    EXPECT_EQ(resources[resourceIndex(Resource::Gold)], 2);
    EXPECT_EQ(resources[resourceIndex(Resource::Gems)], 1);
    EXPECT_EQ(resources[resourceIndex(Resource::Exp)], 15);

    removeFile(filename);
}

TEST(ParserTests, MakesEdgesUndirected) {
    const std::string filename = "undirected_input.txt";

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

    ParseResult result = Parser::parseFile(filename);

    ASSERT_TRUE(result.success);

    const Room& room4 = result.dungeon.getRoom(4);

    std::vector<int> expectedNeighbors = {2, 3, 5};

    EXPECT_EQ(room4.getNeighbors(), expectedNeighbors);

    removeFile(filename);
}

TEST(ParserTests, RejectsInvalidRoomCount) {
    const std::string filename = "invalid_room_count.txt";

    writeFile(
        filename,
        "0\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "0");

    removeFile(filename);
}

TEST(ParserTests, RejectsInvalidRoomLineFormat) {
    const std::string filename = "invalid_room_line.txt";

    writeFile(
        filename,
        "5\n"
        "0 1,2\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2,5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "0 1,2");

    removeFile(filename);
}

TEST(ParserTests, RejectsInvalidNeighborSeparator) {
    const std::string filename = "invalid_neighbor_separator.txt";

    writeFile(
        filename,
        "5\n"
        "0 1,2 0 0 0 0\n"
        "1 0,3 5 2 1 15\n"
        "2 0,4 3 2 1 10\n"
        "3 1,4 1 0 2 40\n"
        "4 2|5 2 4 0 15\n"
        "5 4 0 5 4 10\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "4 2|5 2 4 0 15");

    removeFile(filename);
}

TEST(ParserTests, RejectsResourceValueOutOfRange) {
    const std::string filename = "invalid_resource_value.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 256 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gold\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "1 0,2 5 256 1 15");

    removeFile(filename);
}

TEST(ParserTests, RejectsDuplicateRoomId) {
    const std::string filename = "duplicate_room_id.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "1 0 3 2 1 10\n"
        "6 gold\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "1 0 3 2 1 10");

    removeFile(filename);
}

TEST(ParserTests, RejectsInvalidFood) {
    const std::string filename = "invalid_food.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "1 gold\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "1 gold");

    removeFile(filename);
}

TEST(ParserTests, RejectsInvalidTargetResource) {
    const std::string filename = "invalid_target_resource.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 diamond\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "6 diamond");

    removeFile(filename);
}

TEST(ParserTests, RejectsDuplicateNeighbors) {
    const std::string filename = "duplicate_neighbors.txt";

    writeFile(
        filename,
        "2\n"
        "0 1,1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "0 1,1 0 0 0 0");

    removeFile(filename);
}

TEST(ParserTests, RejectsDuplicateNeighborsInNonStartRoom) {
    const std::string filename = "duplicate_neighbors_non_start.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "1 0,2,2 5 2 1 15");

    removeFile(filename);
}

TEST(ParserTests, RejectsSelfLoop) {
    const std::string filename = "self_loop.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 1,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "1 1,2 5 2 1 15");

    removeFile(filename);
}

TEST(ParserTests, RejectsSelfLoopInStartRoom) {
    const std::string filename = "self_loop_start.txt";

    writeFile(
        filename,
        "2\n"
        "0 0,1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gems\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "0 0,1 0 0 0 0");

    removeFile(filename);
}

TEST(ParserTests, RejectsExtraLineAfterSettings) {
    const std::string filename = "extra_line_after_settings.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gems\n"
        "trash\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.errorLine, "trash");

    removeFile(filename);
}

TEST(ParserTests, AllowsEmptyLinesAfterSettings) {
    const std::string filename = "empty_lines_after_settings.txt";

    writeFile(
        filename,
        "2\n"
        "0 1 0 0 0 0\n"
        "1 0,2 5 2 1 15\n"
        "2 1 3 2 1 10\n"
        "6 gems\n"
        "\n"
        "\n"
    );

    ParseResult result = Parser::parseFile(filename);

    EXPECT_TRUE(result.success);

    removeFile(filename);
}

