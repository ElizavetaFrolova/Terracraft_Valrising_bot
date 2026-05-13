#include <fstream>
#include <string>

#include "bot/AliceStrategy.hpp"
#include "bot/Bot.hpp"
#include "bot/BotController.hpp"
#include "io/Parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    const std::string inputFilename = argv[1];

    std::ofstream output("result.txt");

    if (!output.is_open()) {
        return 1;
    }

    ParseResult parseResult = Parser::parseFile(inputFilename);

    if (!parseResult.success) {
        output << parseResult.errorLine << '\n';
        return 0;
    }

    Dungeon& dungeon = parseResult.dungeon;

    Bot bot(dungeon.getFood(), dungeon.getRoomCount());
    BotController controller(dungeon, bot, output);

    AliceStrategy strategy;
    strategy.run(controller);

    return 0;
}