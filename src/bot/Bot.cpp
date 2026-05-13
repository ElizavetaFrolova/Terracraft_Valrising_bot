#include "bot/Bot.hpp"

Bot::Bot(int initialFood, int roomCount): food(initialFood)
, collectedInRooms(roomCount + 1)
, anyResourceCollectedInRooms(roomCount + 1, false),knowledge(roomCount) {}

int Bot::getCurrentRoom() const {
    return currentRoom;
}

int Bot::getFood() const {
    return food;
}

bool Bot::canSpendFood(int amount) const {
    return amount >= 0 && food >= amount;
}

int Bot::getTotalValue() const {
    return totalValue;
}

const std::array<int, RESOURCE_COUNT>& Bot::getCollectedResources() const {
    return collectedResources;
}

bool Bot::isResourceCollectedInRoom(int roomId, Resource resource) const {
    return collectedInRooms.at(roomId)[resourceIndex(resource)];
}

bool Bot::wasAnyResourceCollectedInRoom(int roomId) const {
    return anyResourceCollectedInRooms.at(roomId);
}

bool Bot::wasAnyResourceCollectedInCurrentRoom() const {
    return wasAnyResourceCollectedInRoom(currentRoom);
}

const std::array<bool, RESOURCE_COUNT>& Bot::getCollectedInRoom(int roomId) const {
    return collectedInRooms.at(roomId);
}

bool Bot::moveTo(int roomId) {
    if (!spendFood()) {
        return false;
    }

    currentRoom = roomId;
    return true;
}

bool Bot::spendFood(int amount) {
    if (!canSpendFood(amount)) {
        return false;
    }

    food -= amount;
    return true;
}

void Bot::collectResource(Resource resource, int amount, int resourceValue) {
    int index = resourceIndex(resource);

    collectedResources[index] += amount;
    totalValue += amount * resourceValue;

    markResourceCollectedInRoom(currentRoom, resource);
}

void Bot::markResourceCollectedInRoom(int roomId, Resource resource) {
    collectedInRooms.at(roomId)[resourceIndex(resource)] = true;
    anyResourceCollectedInRooms.at(roomId) = true;
}

BotKnowledge& Bot::getKnowledge() {
    return knowledge;
}

const BotKnowledge& Bot::getKnowledge() const {
    return knowledge;
}