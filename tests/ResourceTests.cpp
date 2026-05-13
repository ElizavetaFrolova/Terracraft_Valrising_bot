#include <gtest/gtest.h>

#include "model/Resource.hpp"

TEST(ResourceTests, ResourceIndexReturnsCorrectIndexes) {
    EXPECT_EQ(resourceIndex(Resource::Iron), 0);
    EXPECT_EQ(resourceIndex(Resource::Gold), 1);
    EXPECT_EQ(resourceIndex(Resource::Gems), 2);
    EXPECT_EQ(resourceIndex(Resource::Exp), 3);
}

TEST(ResourceTests, ParseResourceReturnsTrueForValidResources) {
    Resource resource;

    EXPECT_TRUE(parseResource("iron", resource));
    EXPECT_EQ(resource, Resource::Iron);

    EXPECT_TRUE(parseResource("gold", resource));
    EXPECT_EQ(resource, Resource::Gold);

    EXPECT_TRUE(parseResource("gems", resource));
    EXPECT_EQ(resource, Resource::Gems);

    EXPECT_TRUE(parseResource("exp", resource));
    EXPECT_EQ(resource, Resource::Exp);
}

TEST(ResourceTests, ParseResourceReturnsFalseForInvalidResource) {
    Resource resource = Resource::Iron;

    EXPECT_FALSE(parseResource("diamond", resource));
    EXPECT_FALSE(parseResource("", resource));
    EXPECT_FALSE(parseResource("Iron", resource));
    EXPECT_FALSE(parseResource("GOLD", resource));
}

TEST(ResourceTests, ResourceNameReturnsCorrectNames) {
    EXPECT_EQ(resourceName(Resource::Iron), "iron");
    EXPECT_EQ(resourceName(Resource::Gold), "gold");
    EXPECT_EQ(resourceName(Resource::Gems), "gems");
    EXPECT_EQ(resourceName(Resource::Exp), "exp");
}