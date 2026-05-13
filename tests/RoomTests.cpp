#include <gtest/gtest.h>

#include <array>
#include <vector>

#include "model/Room.hpp"

TEST(RoomTests, DefaultConstructorCreatesEmptyRoom) {
    Room room;

    EXPECT_EQ(room.getId(), 0);
    EXPECT_TRUE(room.getNeighbors().empty());

    std::array<int, RESOURCE_COUNT> expectedResources{};
    EXPECT_EQ(room.getResources(), expectedResources);
}

TEST(RoomTests, ConstructorWithIdSetsId) {
    Room room(3);

    EXPECT_EQ(room.getId(), 3);
    EXPECT_TRUE(room.getNeighbors().empty());

    std::array<int, RESOURCE_COUNT> expectedResources{};
    EXPECT_EQ(room.getResources(), expectedResources);
}

TEST(RoomTests, FullConstructorSetsAllFields) {
    std::vector<int> neighbors = {1, 4};
    std::array<int, RESOURCE_COUNT> resources = {5, 2, 1, 15};

    Room room(3, neighbors, resources);

    EXPECT_EQ(room.getId(), 3);
    EXPECT_EQ(room.getNeighbors(), neighbors);
    EXPECT_EQ(room.getResources(), resources);
}

TEST(RoomTests, SetNeighborsChangesNeighbors) {
    Room room(1);

    EXPECT_EQ(room.getId(), 1);
    EXPECT_TRUE(room.getNeighbors().empty());

    std::vector<int> neighbors = {0, 3};

    room.setNeighbors(neighbors);

    EXPECT_EQ(room.getNeighbors(), neighbors);
}

TEST(RoomTests, SetResourcesChangesResources) {
    Room room(1);

    std::array<int, RESOURCE_COUNT> resources = {5, 2, 1, 15};

    room.setResources(resources);

    EXPECT_EQ(room.getResources(), resources);
}

TEST(RoomTests, AddNeighborAddsOneNeighbor) {
    Room room(1);

    room.addNeighbor(3);

    std::vector<int> expected = {3};

    EXPECT_EQ(room.getNeighbors(), expected);
}

TEST(RoomTests, AddNeighborAddsSeveralNeighborsInOrder) {
    Room room(1);

    room.addNeighbor(3);
    room.addNeighbor(5);
    room.addNeighbor(2);

    std::vector<int> expected = {3, 5, 2};

    EXPECT_EQ(room.getNeighbors(), expected);
}

TEST(RoomTests, SortNeighborsSortsAscending) {
    Room room(1);

    room.addNeighbor(5);
    room.addNeighbor(2);
    room.addNeighbor(4);
    room.addNeighbor(1);

    room.sortNeighbors();

    std::vector<int> expected = {1, 2, 4, 5};

    EXPECT_EQ(room.getNeighbors(), expected);
}

TEST(RoomTests, HasResourceReturnsTrueWhenResourceExists) {
    std::vector<int> neighbors = {0};
    std::array<int, RESOURCE_COUNT> resources = {5, 0, 2, 1};

    Room room(1, neighbors, resources);

    EXPECT_TRUE(room.hasResource(Resource::Iron));
    EXPECT_FALSE(room.hasResource(Resource::Gold));
    EXPECT_TRUE(room.hasResource(Resource::Gems));
    EXPECT_TRUE(room.hasResource(Resource::Exp));
}

TEST(RoomTests, CollectResourceDecreasesResourceCount) {
    std::vector<int> neighbors = {0};
    std::array<int, RESOURCE_COUNT> resources = {5, 2, 1, 15};

    Room room(1, neighbors, resources);

    EXPECT_EQ(room.collectResource(Resource::Gems), 1);

    const auto& currentResources = room.getResources();

    EXPECT_EQ(currentResources[resourceIndex(Resource::Iron)], 5);
    EXPECT_EQ(currentResources[resourceIndex(Resource::Gold)], 2);
    EXPECT_EQ(currentResources[resourceIndex(Resource::Gems)], 0);
    EXPECT_EQ(currentResources[resourceIndex(Resource::Exp)], 15);
}

TEST(RoomTests, CollectResourceReturnsZeroWhenResourceDoesNotExist) {
    std::vector<int> neighbors = {0};
    std::array<int, RESOURCE_COUNT> resources = {5, 0, 0, 15};

    Room room(1, neighbors, resources);

    EXPECT_EQ(room.collectResource(Resource::Gold), 0);
    EXPECT_EQ(room.collectResource(Resource::Gems), 0);

    const auto& currentResources = room.getResources();

    EXPECT_EQ(currentResources[resourceIndex(Resource::Iron)], 5);
    EXPECT_EQ(currentResources[resourceIndex(Resource::Gold)], 0);
    EXPECT_EQ(currentResources[resourceIndex(Resource::Gems)], 0);
    EXPECT_EQ(currentResources[resourceIndex(Resource::Exp)], 15);
}

TEST(RoomTests, CollectResourceCollectsAllAmountAtOnce) {
    std::vector<int> neighbors = {0};
    std::array<int, RESOURCE_COUNT> resources = {0, 3, 0, 0};

    Room room(1, neighbors, resources);

    EXPECT_EQ(room.collectResource(Resource::Gold), 3);
    EXPECT_EQ(room.collectResource(Resource::Gold), 0);

    const auto& currentResources = room.getResources();

    EXPECT_EQ(currentResources[resourceIndex(Resource::Gold)], 0);
}