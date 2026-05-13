#include "model/Dungeon.hpp"

int Dungeon::getRoomCount() const {
    return roomCount;
}

int Dungeon::getFood() const {
    return food;
}

Resource Dungeon::getTargetResource() const {
    return targetResource;
}

const std::array<int, RESOURCE_COUNT>&Dungeon::getResourceValues() const {
    return resourceValues;
}

const std::vector<Room>& Dungeon::getRooms() const {
    return rooms;
}

Room& Dungeon::getRoom(int id) {
    return rooms.at(id);
}

const Room& Dungeon::getRoom(int id) const {
    return rooms.at(id);
}

void Dungeon::setRoomCount(int count) {
    roomCount = count;
}

void Dungeon::setFood(int newFood) {
    food = newFood;
}

void Dungeon::setTargetResource(Resource resource) {
    targetResource = resource;
    resourceValues = {7, 11, 23, 1};
    resourceValues[resourceIndex(resource)] *= 2;
}

void Dungeon::setRooms(const std::vector<Room>& newRooms) {
    rooms = newRooms;
}

void Dungeon::addRoom(const Room& room) {
    rooms.push_back(room);
}