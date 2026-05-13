#pragma once

#include <vector>

#include "bot/BotKnowledge.hpp"
#include "model/Dungeon.hpp"

class KnownDungeonView {
private:
    const Dungeon& dungeon;
    const BotKnowledge& knowledge;

public:
    KnownDungeonView(const Dungeon& dungeon, const BotKnowledge& knowledge);

    int getRoomCount() const;

    bool isVisited(int roomId) const;
    bool canSeeNeighbors(int roomId) const;

    std::vector<int> getNeighbors(int roomId) const;
};