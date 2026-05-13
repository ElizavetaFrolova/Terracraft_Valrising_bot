#include "algorithms/PathFinder.hpp"

#include <algorithm>
#include <queue>

std::vector<int> PathFinder::restorePath(int finish, const std::vector<int>& parent) {
    std::vector<int> path;

    for (int current = finish; current != -1; current = parent[current]) {
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<int> PathFinder::findPathToNearestUnvisitedRoom(const KnownDungeonView& view
    , int start) {
    int roomCount = view.getRoomCount() + 1;

    std::vector<bool> visited(roomCount, false);
    std::vector<int> parent(roomCount, -1);
    std::vector<int> distance(roomCount, -1);

    std::queue<int> queue;

    visited[start] = true;
    distance[start] = 0;
    queue.push(start);

    int bestRoom = -1;
    int bestDistance = -1;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (bestDistance != -1 && distance[current] > bestDistance) {
            break;
        }

        if (!view.isVisited(current)) {
            if (bestRoom == -1 || current < bestRoom) {
                bestRoom = current;
                bestDistance = distance[current];
            }

            continue;
        }

        if (!view.canSeeNeighbors(current)) {
            continue;
        }

        for (int neighbor : view.getNeighbors(current)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                distance[neighbor] = distance[current] + 1;
                queue.push(neighbor);
            }
        }
    }

    if (bestRoom == -1) {
        return {};
    }

    return restorePath(bestRoom, parent);
}

std::vector<int> PathFinder::findShortestPathThroughVisitedRooms(const KnownDungeonView& view
    , int start, int finish) {
    int roomCount = view.getRoomCount() + 1;

    std::vector<bool> visited(roomCount, false);
    std::vector<int> parent(roomCount, -1);

    std::queue<int> queue;

    visited[start] = true;
    queue.push(start);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (current == finish) {
            break;
        }

        for (int neighbor : view.getNeighbors(current)) {
            if (!view.isVisited(neighbor)) {
                continue;
            }

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                queue.push(neighbor);
            }
        }
    }

    if (!visited[finish]) {
        return {};
    }

    return restorePath(finish, parent);
}