#include "io/Parser.hpp"

#include <array>
#include <fstream>
#include <sstream>

bool Parser::isInRange(int value, int left, int right) {
    return value >= left && value <= right;
}

std::vector<std::string> Parser::splitBySpaces(const std::string& line) {
    
    std::vector<std::string> parts;

    std::stringstream stream(line);
    std::string part;

    while (std::getline(stream, part, ' ')) {
        if (!part.empty()) {
            parts.push_back(part);
        }
    }

    return parts;
}

bool Parser::parseInteger(const std::string& text, int& value) {
    if (text.empty()) {
        return false;
    }

    int result = 0;

    for (char symbol : text) {
        if (symbol < '0' || symbol > '9') {
            return false;
        }

        result = result * 10 + (symbol - '0');

        if(result>255){
            return false;
        }
    }

    value = result;
    return true;
}

bool Parser::parseNeighbors(const std::string& text, int roomId
    , int maxRoomId, std::vector<int>& neighbors) {
    std::stringstream stream(text);
    std::string part;

    std::vector<bool> usedNeighbors(maxRoomId + 1, false);

    while (std::getline(stream, part, ',')) {
        if (part.empty()) {
            return false;
        }

        int neighbor = 0;

        if (!parseInteger(part, neighbor)) {
            return false;
        }

        if (!isInRange(neighbor, 0, maxRoomId)) {
            return false;
        }

        if (neighbor == roomId) {
            return false;
        }

        if (usedNeighbors[neighbor]) {
            return false;
        }

        usedNeighbors[neighbor] = true;
        neighbors.push_back(neighbor);
    }

    return !neighbors.empty();
}

bool Parser::parseRoomLine(const std::string& line, int maxRoomId, Room& room) {
    std::vector<std::string> parts = splitBySpaces(line);

    if (parts.size() != 6) {
        return false;
    }

    int id = 0;

    if (!parseInteger(parts[0], id)) {
        return false;
    }

    if (!isInRange(id, 0, maxRoomId)) {
        return false;
    }

    std::vector<int> neighbors;

    if (!parseNeighbors(parts[1], id, maxRoomId, neighbors)) {
        return false;
    }

    std::array<int, RESOURCE_COUNT> resources{};

    for (int i = 0; i < RESOURCE_COUNT; ++i) {
        if (!parseInteger(parts[i + 2], resources[i])) {
            return false;
        }

        if (!isInRange(resources[i], 0, 255)) {
            return false;
        }
    }

    room = Room(id, neighbors, resources);

    return true;
}

bool Parser::parseSettingsLine(const std::string& line, int& food, Resource& targetResource) {
    std::vector<std::string> parts = splitBySpaces(line);

    if (parts.size() != 2) {
        return false;
    }

    if (!parseInteger(parts[0], food)) {
        return false;
    }

    if (!isInRange(food, 2, 255)) {
        return false;
    }

    if (!parseResource(parts[1], targetResource)) {
        return false;
    }

    return true;
}

bool Parser::hasNeighbor(const Room& room, int neighborId) {
    for (int neighbor : room.getNeighbors()) {
        if (neighbor == neighborId) {
            return true;
        }
    }

    return false;
}

void Parser::normalizeEdges(std::vector<Room>& rooms) {
    for (int from = 0; from < static_cast<int>(rooms.size()); ++from) {
        const std::vector<int>& neighbors = rooms[from].getNeighbors();

        for (int to : neighbors) {
            if (!hasNeighbor(rooms[to], from)) {
                rooms[to].addNeighbor(from);
            }
        }
    }

    for (Room& room : rooms) {
        room.sortNeighbors();
    }
}

ParseResult Parser::parseFile(const std::string& filename) {
    ParseResult result;

    std::ifstream input(filename);

    if (!input.is_open()) {
        result.success = false;
        result.errorLine = "";

        return result;
    }

    std::string line;

    if (!std::getline(input, line)) {
        result.success = false;
        result.errorLine = "";

        return result;
    }

    int roomCount = 0;

    if (!parseInteger(line, roomCount) || !isInRange(roomCount, 1, 255)) {

        result.success = false;
        result.errorLine = line;

        return result;
    }

    Dungeon dungeon;

    dungeon.setRoomCount(roomCount);

    std::vector<Room> rooms(roomCount + 1);
    std::vector<bool> usedIds(roomCount + 1, false);

    for (int i = 0; i < roomCount + 1; ++i) {
        if (!std::getline(input, line)) {
            result.success = false;
            result.errorLine = "";

            return result;
        }

        Room room;

        if (!parseRoomLine(line, roomCount, room)) {
            result.success = false;
            result.errorLine = line;

            return result;
        }

        int id = room.getId();

        if (usedIds[id]) {
            result.success = false;
            result.errorLine = line;

            return result;
        }

        usedIds[id] = true;
        rooms[id] = room;
    }

    for (bool used : usedIds) {
        if (!used) {
            result.success = false;
            result.errorLine = "";

            return result;
        }
    }

    normalizeEdges(rooms);

    if (!std::getline(input, line)) {
        result.success = false;
        result.errorLine = "";

        return result;
    }

    int food = 0;
    Resource targetResource{};

    if (!parseSettingsLine(line, food, targetResource)) {
        result.success = false;
        result.errorLine = line;

        return result;
    }

    while (std::getline(input, line)) {
        if (!line.empty()) {
            result.success = false;
            result.errorLine = line;

            return result;
        }
    }

    dungeon.setRooms(rooms);
    dungeon.setFood(food);
    dungeon.setTargetResource(targetResource);

    result.success = true;
    result.dungeon = dungeon;

    return result;
}