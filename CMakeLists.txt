cmake_minimum_required(VERSION 3.22)

project(
        pathfinding
        VERSION 0.1
        LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pathfinding Core/src/main.cpp)