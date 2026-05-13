#pragma once

#include <array>
#include <ostream>
#include <vector>

#include "bot/Bot.hpp"
#include "model/Dungeon.hpp"
#include "model/Resource.hpp"

class BotController {
private:
    Dungeon& dungeon;
    Bot& bot;
    std::ostream& out;

    void revealAfterVisit(int roomId);

public:
    BotController(Dungeon& dungeon, Bot& bot, std::ostream& out);

    int getCurrentRoom() const;
    int getFood() const;
    int getInitialFood() const;

    bool willCurrentRoomCollectionSpendFood() const;

    const BotKnowledge& getKnowledge() const;

    std::vector<int> getKnownNeighbors(int roomId) const;

    std::array<int, RESOURCE_COUNT> getCurrentRoomResources() const;
    const std::array<int, RESOURCE_COUNT>& getResourceValues() const;

    std::vector<int> findPathToNearestUnvisitedRoom() const;
    std::vector<int> findReturnPathToStart() const;

    bool moveTo(int roomId);
    bool collect(Resource resource);

    void printState() const;
    void printResult() const;
};