#pragma once

#include <string>
#include <vector>

#include "model/Dungeon.hpp"
#include "model/Room.hpp"

struct ParseResult {
    bool success = false;
    Dungeon dungeon;
    std::string errorLine;
};

class Parser {
public:
    static ParseResult parseFile(const std::string& filename);

private:
    static bool isInRange(int value, int left, int right);

    static std::vector<std::string> splitBySpaces(const std::string& line);

    static bool parseInteger(const std::string& text, int& value);

    static bool parseNeighbors(const std::string& text, int roomId
        , int maxRoomId, std::vector<int>& neighbors);

    static bool parseRoomLine(const std::string& line, int maxRoomId, Room& room);

    static bool parseSettingsLine(const std::string& line, int& food, Resource& targetResource);

    static bool hasNeighbor(const Room& room, int neighborId);

    static void normalizeEdges(std::vector<Room>& rooms);
};