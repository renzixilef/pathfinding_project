cmake_minimum_required(VERSION 3.22)

project(
        pathfinding
        VERSION 0.1
        LANGUAGES C CXX ASM
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

file(GLOB_RECURSE MOC_HEADERS
        Core/inc/gui_*.h
        Core/inc/RunInterface/runner.h
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

file(GLOB_RECURSE SRC
        Core/src/*.cpp
)

add_executable(pathfinding ${SRC} ${MOC_SRCS})
target_include_directories(pathfinding PUBLIC Core/inc)

target_link_libraries(pathfinding Qt5::Widgets Qt5::Core)