#pragma once

#include <array>
#include <vector>


#include "model/Resource.hpp"

class Room {
private:

    int id{};
    std::vector<int> neighbors;
    std::array<int, RESOURCE_COUNT> resources{};

public:
    Room() = default;
    explicit Room(int id);
    Room(int id,const std::vector<int>& neighbors,const std::array<int, RESOURCE_COUNT>& resources);

    int getId() const;
    const std::vector<int>& getNeighbors() const;
    const std::array<int, RESOURCE_COUNT>& getResources() const;

    void setNeighbors(const std::vector<int>& neighbors);
    void setResources(const std::array<int, RESOURCE_COUNT>& resources);
    void addNeighbor(int neighbor);
    void sortNeighbors();

    bool hasResource(Resource resource) const;
    int collectResource(Resource resource);

};