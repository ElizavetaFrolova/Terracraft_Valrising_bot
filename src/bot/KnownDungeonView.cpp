#include "bot/KnownDungeonView.hpp"

KnownDungeonView::KnownDungeonView(const Dungeon& dungeon, const BotKnowledge& knowledge)
    : dungeon(dungeon), knowledge(knowledge) {}

int KnownDungeonView::getRoomCount() const {
    return dungeon.getRoomCount();
}

bool KnownDungeonView::isVisited(int roomId) const {
    return knowledge.isVisited(roomId);
}

bool KnownDungeonView::canSeeNeighbors(int roomId) const {
    return knowledge.canSeeNeighbors(roomId);
}

std::vector<int> KnownDungeonView::getNeighbors(int roomId) const {
    if (!canSeeNeighbors(roomId)) {
        return {};
    }

    return dungeon.getRoom(roomId).getNeighbors();
}