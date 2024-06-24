cmake_minimum_required(VERSION 3.22)

# project setup
project(
        pathfinding
        VERSION 0.1
        LANGUAGES C CXX ASM
)

enable_testing()
include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include qt5 libraries
#set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# include qt5 relevant headers and moc them
file(GLOB_RECURSE MOC_HEADERS
        Core/inc/gui_*.h
        Core/inc/RunInterface/runner.h
)

qt5_wrap_cpp(MOC_SRCS ${MOC_HEADERS})

# find all relevant source files
file(GLOB_RECURSE SRC
        Core/src/*.cpp
)

# setup target and linker
add_executable(pathfinding ${SRC} ${MOC_SRCS})
target_include_directories(pathfinding PUBLIC Core/inc)

target_link_libraries(pathfinding Qt5::Widgets Qt5::Core)

# doxygen documentation
find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_command(TARGET pathfinding POST_BUILD
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )

    find_program(MAKE_EXECUTABLE make)
    if (MAKE_EXECUTABLE)
        add_custom_command(TARGET pathfinding POST_BUILD
                COMMAND ${MAKE_EXECUTABLE} > /dev/null 2>&1
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/latex
                COMMENT "Generating PDF documentation"
                VERBATIM)
    else (MAKE_EXECUTABLE)
        message("Could not find 'make' command for generating PDF documentation.")
    endif ()
else (DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# testing
# set up gtest
find_package(GTest)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(AStarPathfinder_UnitTest test/AStarPathfinder_UnitTest)
    target_link_libraries(AStarPathfinder_UnitTest gtest gtest_main pthread)
    add_test(NAME AStarPathFinder_Test COMMAND AStarPathFinder_UnitTest)
else (GTEST_FOUND)
    message("Could not find gtest, skipping tests.")
endif (GTEST_FOUND)