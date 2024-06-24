cmake_minimum_required(VERSION 3.22)

# project setup
project(
        pathfinding
        VERSION 0.1
        LANGUAGES C CXX ASM
)

enable_testing()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include qt5 libraries
#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_subdirectory(Core/GridGenerator)
add_subdirectory(Core/Pathfinder)
add_subdirectory(Core/RunInterface)
add_subdirectory(Core/GUI/widgets)
add_subdirectory(Core/GUI)


add_executable(pathfinding Core/main.cpp)
target_link_libraries(pathfinding GridGenerator Pathfinder GUI RunInterface GUI_widgets)




