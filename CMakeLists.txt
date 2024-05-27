cmake_minimum_required(VERSION 3.22)

project(
        pathfinding
        VERSION 0.1
        LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SRC
        Core/src/*.cpp
)

add_executable(pathfinding ${SRC})

target_link_libraries(pathfinding PRIVATE Qt5::Widgets)