#include "model/Resource.hpp"

int resourceIndex(Resource resource) {
    return static_cast<int>(resource);
}

bool parseResource(const std::string& text, Resource& resource) {
    if (text == "iron") {
        resource = Resource::Iron;
        return true;
    }

    if (text == "gold") {
        resource = Resource::Gold;
        return true;
    }

    if (text == "gems") {
        resource = Resource::Gems;
        return true;
    }

    if (text == "exp") {
        resource = Resource::Exp;
        return true;
    }

    return false;
}

std::string resourceName(Resource resource) {
    switch (resource) {
        case Resource::Iron:
            return "iron";
        case Resource::Gold:
            return "gold";
        case Resource::Gems:
            return "gems";
        case Resource::Exp:
            return "exp";
    }

    return "";
}