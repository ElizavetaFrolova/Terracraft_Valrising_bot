#include "bot/BotKnowledge.hpp"

BotKnowledge::BotKnowledge(int roomCount)
    : roomLevels(roomCount + 1, RoomKnowledgeLevel::Unknown) {
}

RoomKnowledgeLevel BotKnowledge::getRoomLevel(int roomId) const {
    return roomLevels.at(roomId);
}

void BotKnowledge::upgradeRoom(int roomId, RoomKnowledgeLevel newLevel) {
    if (static_cast<int>(newLevel) > static_cast<int>(roomLevels.at(roomId))) {
        roomLevels.at(roomId) = newLevel;
    }
}

bool BotKnowledge::isUnknown(int roomId) const {
    return roomLevels.at(roomId) == RoomKnowledgeLevel::Unknown;
}

bool BotKnowledge::isNumberKnown(int roomId) const {
    return roomLevels.at(roomId) == RoomKnowledgeLevel::NumberKnown;
}

bool BotKnowledge::isVisible(int roomId) const {
    return roomLevels.at(roomId) == RoomKnowledgeLevel::Visible;
}

bool BotKnowledge::isVisited(int roomId) const {
    return roomLevels.at(roomId) == RoomKnowledgeLevel::Visited;
}

bool BotKnowledge::canSeeRoomNumber(int roomId) const {
    return !isUnknown(roomId);
}

bool BotKnowledge::canSeeNeighbors(int roomId) const {
    return isVisible(roomId) || isVisited(roomId);
}

bool BotKnowledge::canSeeResources(int roomId) const {
    return isVisited(roomId);
}

std::vector<bool> BotKnowledge::getVisitedRooms() const {
    std::vector<bool> visitedRooms(roomLevels.size(), false);

    for (std::size_t i = 0; i < roomLevels.size(); ++i) {
        visitedRooms[i] = roomLevels[i] == RoomKnowledgeLevel::Visited;
    }

    return visitedRooms;
}

void BotKnowledge::markNumberKnown(int roomId) {
    upgradeRoom(roomId, RoomKnowledgeLevel::NumberKnown);
}

void BotKnowledge::markVisible(int roomId) {
    upgradeRoom(roomId, RoomKnowledgeLevel::Visible);
}

void BotKnowledge::markVisited(int roomId) {
    upgradeRoom(roomId, RoomKnowledgeLevel::Visited);
}