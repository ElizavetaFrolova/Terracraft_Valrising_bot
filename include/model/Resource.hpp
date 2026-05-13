#pragma once

#include <string>

constexpr int RESOURCE_COUNT = 4;

enum class Resource {
    Iron,
    Gold,
    Gems,
    Exp
};

int resourceIndex(Resource resource);

bool parseResource(const std::string& text, Resource& resource);

std::string resourceName(Resource resource);