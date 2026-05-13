cmake_minimum_required(VERSION 3.16)

project(TerracraftBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(terracraft_lib
    src/model/Resource.cpp
    src/model/Room.cpp
    src/model/Dungeon.cpp
    src/io/Parser.cpp
    src/bot/Bot.cpp
    src/algorithms/PathFinder.cpp
    src/bot/AliceStrategy.cpp
    src/bot/BotKnowledge.cpp
    src/bot/BotController.cpp
    src/bot/KnownDungeonView.cpp
)

target_include_directories(terracraft_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(task
    src/main.cpp
)

target_link_libraries(task PRIVATE
    terracraft_lib
)

add_subdirectory(googletest)
add_subdirectory(tests)