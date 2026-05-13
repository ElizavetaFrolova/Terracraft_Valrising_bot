#pragma once

#include <array>

#include "bot/BotController.hpp"
#include "bot/Strategy.hpp"
#include "model/Resource.hpp"

class AliceStrategy : public Strategy {
public:
    void run(BotController& controller) override;

private:
    void explorationPhase(BotController& controller);

    void returnPhase(BotController& controller);

    static bool chooseBestResource(const std::array<int
        , RESOURCE_COUNT>& resources, const std::array<int
        , RESOURCE_COUNT>& values, Resource& bestResource);
};