#include "bot/AliceStrategy.hpp"

#include <vector>

void AliceStrategy::run(BotController& controller) {
    explorationPhase(controller);
    returnPhase(controller);

    controller.printResult();
}

void AliceStrategy::explorationPhase(BotController& controller) {
    int explorationFood = controller.getInitialFood() / 2;

    while (explorationFood > 0) {
        int currentRoomId = controller.getCurrentRoom();

        std::vector<int> path;

        for (int neighbor : controller.getKnownNeighbors(currentRoomId)) {
            if (!controller.getKnowledge().isVisited(neighbor)) {
                path = {currentRoomId, neighbor};
                break;
            }
        }

        if (path.empty()) {
            path = controller.findPathToNearestUnvisitedRoom();
        }

        if (path.size() < 2) {
            break;
        }

        for (std::size_t i = 1; i < path.size(); ++i) {
            if (explorationFood <= 0) {
                return;
            }

            if (!controller.moveTo(path[i])) {
                return;
            }

            --explorationFood;

            controller.printState();

            Resource bestResource;

            if (chooseBestResource(controller.getCurrentRoomResources()
            , controller.getResourceValues(), bestResource)) {
                bool needsFood = controller.willCurrentRoomCollectionSpendFood();

                if (!needsFood || explorationFood > 0) {
                    if (controller.collect(bestResource)) {
                        if (needsFood) {
                            --explorationFood;
                        }

                        controller.printState();
                    }
                }
            }
        }
    }
}

void AliceStrategy::returnPhase(BotController& controller) {
    std::vector<int> path = controller.findReturnPathToStart();

    if (path.size() < 2) {
        return;
    }

    int movesToStart = static_cast<int>(path.size()) - 1;

    for (std::size_t i = 1; i < path.size(); ++i) {
        while (controller.getFood() > movesToStart) {
            Resource bestResource;

            if (!chooseBestResource(
                    controller.getCurrentRoomResources(),
                    controller.getResourceValues(),
                    bestResource
                )) {
                break;
            }

            if (!controller.collect(bestResource)) {
                break;
            }

            controller.printState();
        }

        if (!controller.moveTo(path[i])) {
            return;
        }

        --movesToStart;

        if (controller.getCurrentRoom() != 0) {
            controller.printState();
        }
    }
}

bool AliceStrategy::chooseBestResource(
    const std::array<int, RESOURCE_COUNT>& resources,
    const std::array<int, RESOURCE_COUNT>& values,
    Resource& bestResource
) {
    bool found = false;
    int bestValue = -1;

    for (int i = 0; i < RESOURCE_COUNT; ++i) {
        if (resources[i] <= 0) {
            continue;
        }

        if (!found || values[i] > bestValue) {
            found = true;
            bestValue = values[i];
            bestResource = static_cast<Resource>(i);
        }
    }

    return found;
}