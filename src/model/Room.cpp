#include "model/Room.hpp"

#include <algorithm>

Room::Room(int id): id(id) {}

Room::Room(int id, const std::vector<int>& neighbors,const std::array<int, RESOURCE_COUNT>& resources)
    : id(id),neighbors(neighbors),resources(resources) {}

int Room::getId() const {
    return id;
}

const std::vector<int>& Room::getNeighbors() const {
    return neighbors;
}

const std::array<int, RESOURCE_COUNT>& Room::getResources() const {
    return resources;
}

void Room::setNeighbors(const std::vector<int>& newNeighbors) {
    neighbors = newNeighbors;
}

void Room::setResources(const std::array<int, RESOURCE_COUNT>& newResources) {
    resources = newResources;
}

void Room::addNeighbor(int neighbor) {
    neighbors.push_back(neighbor);
}

void Room::sortNeighbors() {
    std::sort(neighbors.begin(), neighbors.end());
}

bool Room::hasResource(Resource resource) const {
    int index = resourceIndex(resource);

    return resources[index] > 0;
}

int Room::collectResource(Resource resource) {
    int index = resourceIndex(resource);

    if (resources[index] <= 0) {
        return 0;
    }

    int collectedAmount = resources[index];
    resources[index] = 0;

    return collectedAmount;
}

