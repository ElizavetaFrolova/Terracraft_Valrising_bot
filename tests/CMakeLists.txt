add_executable(terracraft_tests
    ResourceTests.cpp
    RoomTests.cpp
    DungeonTests.cpp
    ParserTests.cpp
    BotTests.cpp
    BotKnowledgeTests.cpp
    PathFinderTests.cpp
    AliceStrategyTests.cpp
    KnownDungeonViewTests.cpp
    BotControllerTests.cpp
)

target_link_libraries(terracraft_tests PRIVATE
    terracraft_lib
    gtest
    gtest_main
)

target_include_directories(terracraft_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)