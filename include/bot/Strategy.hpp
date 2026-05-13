#pragma once

class BotController;

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual void run(BotController& controller) = 0;
};