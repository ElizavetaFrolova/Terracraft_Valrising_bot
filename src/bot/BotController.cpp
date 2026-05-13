#include "bot/BotController.hpp"

#include "algorithms/PathFinder.hpp"
#include "bot/KnownDungeonView.hpp"

BotController::BotController(Dungeon& dungeon, Bot& bot, std::ostream& out)
    : dungeon(dungeon), bot(bot), out(out) {
    revealAfterVisit(bot.getCurrentRoom());
}



int BotController::getCurrentRoom() const {
    return bot.getCurrentRoom();
}

int BotController::getFood() const {
    return bot.getFood();
}

int BotController::getInitialFood() const {
    return dungeon.getFood();
}

bool BotController::willCurrentRoomCollectionSpendFood() const {
    return bot.wasAnyResourceCollectedInCurrentRoom();
}

const BotKnowledge& BotController::getKnowledge() const {
    return bot.getKnowledge();
}

std::vector<int> BotController::getKnownNeighbors(int roomId) const {
    KnownDungeonView view(dungeon, bot.getKnowledge());
    return view.getNeighbors(roomId);
}

std::array<int, RESOURCE_COUNT> BotController::getCurrentRoomResources() const {
    return dungeon.getRoom(bot.getCurrentRoom()).getResources();
}

const std::array<int, RESOURCE_COUNT>& BotController::getResourceValues() const {
    return dungeon.getResourceValues();
}

std::vector<int> BotController::findPathToNearestUnvisitedRoom() const {
    KnownDungeonView view(dungeon, bot.getKnowledge());

    return PathFinder::findPathToNearestUnvisitedRoom(view, bot.getCurrentRoom());
}

std::vector<int> BotController::findReturnPathToStart() const {
    KnownDungeonView view(dungeon, bot.getKnowledge());

    return PathFinder::findShortestPathThroughVisitedRooms(view
        , bot.getCurrentRoom(),0);
}

bool BotController::moveTo(int roomId) {
    if (!bot.moveTo(roomId)) {
        return false;
    }

    out << "go " << roomId << '\n';

    revealAfterVisit(roomId);

    return true;
}

bool BotController::collect(Resource resource) {
    Room& room = dungeon.getRoom(bot.getCurrentRoom());

    if (!room.hasResource(resource)) {
        return false;
    }

    bool needsFood = bot.wasAnyResourceCollectedInCurrentRoom();

    if (needsFood && !bot.spendFood()) {
        return false;
    }

    int collectedAmount = room.collectResource(resource);

    if (collectedAmount == 0) {
        return false;
    }

    int value = dungeon.getResourceValues()[resourceIndex(resource)];

    bot.collectResource(resource, collectedAmount, value);

    out << "collect " << resourceName(resource) << '\n';

    return true;
}

void BotController::printState() const {
    int roomId = bot.getCurrentRoom();

    const Room& room = dungeon.getRoom(roomId);
    const auto& resources = room.getResources();

    out << "state " << roomId;

    for (int i = 0; i < RESOURCE_COUNT; ++i) {
        Resource resource = static_cast<Resource>(i);

        out << ' ';

        if (bot.isResourceCollectedInRoom(roomId, resource)) {
            out << '_';
        } else {
            out << resources[i];
        }
    }

    out << '\n';
}

void BotController::printResult() const {
    const auto& collected = bot.getCollectedResources();

    out << "result";

    for (int i = 0; i < RESOURCE_COUNT; ++i) {
        out << ' ' << collected[i];
    }

    out << ' ' << bot.getTotalValue() << '\n';
}

void BotController::revealAfterVisit(int roomId) {
    BotKnowledge& knowledge = bot.getKnowledge();

    knowledge.markVisited(roomId);

    const Room& visitedRoom = dungeon.getRoom(roomId);

    for (int visibleRoomId : visitedRoom.getNeighbors()) {
        knowledge.markVisible(visibleRoomId);

        const Room& visibleRoom = dungeon.getRoom(visibleRoomId);

        for (int knownRoomId : visibleRoom.getNeighbors()) {
            knowledge.markNumberKnown(knownRoomId);
        }
    }
}