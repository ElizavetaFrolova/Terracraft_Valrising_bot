#pragma once

#include <array>
#include <vector>

#include "model/Resource.hpp"
#include "bot/BotKnowledge.hpp"

class Bot {
private:
    int currentRoom = 0;
    int food = 0;

    std::array<int, RESOURCE_COUNT> collectedResources{};
    int totalValue = 0;

    std::vector<std::array<bool, RESOURCE_COUNT>> collectedInRooms;
    std::vector<bool> anyResourceCollectedInRooms;

    BotKnowledge knowledge;

    void markResourceCollectedInRoom(int roomId, Resource resource);

public:
    Bot(int initialFood, int roomCount);

    int getCurrentRoom() const;
    int getFood() const;
    int getTotalValue() const;

    bool canSpendFood(int amount = 1) const;

    const std::array<int, RESOURCE_COUNT>& getCollectedResources() const;

    BotKnowledge& getKnowledge();
    const BotKnowledge& getKnowledge() const;

    bool isResourceCollectedInRoom(int roomId, Resource resource) const;
    bool wasAnyResourceCollectedInRoom(int roomId) const;
    bool wasAnyResourceCollectedInCurrentRoom() const;

    const std::array<bool, RESOURCE_COUNT>& getCollectedInRoom(int roomId) const;

    bool moveTo(int roomId);
    bool spendFood(int amount = 1);

    void collectResource(Resource resource, int amount, int resourceValue);
};