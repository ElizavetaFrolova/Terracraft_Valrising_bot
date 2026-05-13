#pragma once

#include <vector>

#include "bot/KnownDungeonView.hpp"

class PathFinder {
public:
    static std::vector<int> findPathToNearestUnvisitedRoom(const KnownDungeonView& view
        , int start);

    static std::vector<int> findShortestPathThroughVisitedRooms(const KnownDungeonView& view
        , int start, int finish);

private:
    static std::vector<int> restorePath(int finish
        , const std::vector<int>& parent);
};