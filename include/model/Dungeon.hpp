#pragma once

#include <array>
#include <vector>

#include "model/Resource.hpp"
#include "model/Room.hpp"

class Dungeon {
private:

    int roomCount{};
    int food{};
    Resource targetResource{};

    std::array<int, RESOURCE_COUNT> resourceValues{7, 11, 23, 1};

    std::vector<Room> rooms;
public:

    int getRoomCount() const;
    int getFood() const;
    Resource getTargetResource() const;
    const std::array<int, RESOURCE_COUNT>& getResourceValues() const;
    const std::vector<Room>& getRooms() const;
    Room& getRoom(int id);
    const Room& getRoom(int id) const;

    void setRoomCount(int count);
    void setFood(int food);
    void setTargetResource(Resource resource);
    void setRooms(const std::vector<Room>& rooms);
    void addRoom(const Room& room);

};