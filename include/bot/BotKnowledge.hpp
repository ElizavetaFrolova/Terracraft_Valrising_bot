#pragma once

#include <vector>

enum class RoomKnowledgeLevel {
    Unknown = 0,
    NumberKnown = 1,
    Visible = 2,
    Visited = 3
};

class BotKnowledge {
private:
    std::vector<RoomKnowledgeLevel> roomLevels;

    void upgradeRoom(int roomId, RoomKnowledgeLevel newLevel);

public:
    explicit BotKnowledge(int roomCount);

    RoomKnowledgeLevel getRoomLevel(int roomId) const;

    bool isUnknown(int roomId) const;
    bool isNumberKnown(int roomId) const;
    bool isVisible(int roomId) const;
    bool isVisited(int roomId) const;

    bool canSeeRoomNumber(int roomId) const;
    bool canSeeNeighbors(int roomId) const;
    bool canSeeResources(int roomId) const;

    std::vector<bool> getVisitedRooms() const;

    void markNumberKnown(int roomId);
    void markVisible(int roomId);
    void markVisited(int roomId);
};