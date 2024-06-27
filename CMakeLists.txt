cmake_minimum_required(VERSION 3.22)

# project setup
project(
        Pathfinding_Bench_Program
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

# include opencv
set(OpenCV_DIR "/usr/lib/x86_64-linux-gnu/cmake/opencv4")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# include project-own libraries
add_subdirectory(Core/GridGenerator)
add_subdirectory(Core/Pathfinder)
add_subdirectory(Core/RunInterface)
add_subdirectory(Core/GUI/widgets)
add_subdirectory(Core/GUI)

# main executable setup
add_executable(Pathfinding_Bench_Program Core/main.cpp)
target_link_libraries(Pathfinding_Bench_Program GridGenerator Pathfinder GUI RunInterface GUI_widgets ${OpenCV_LIBS})

# doxygen documentation
find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_command(TARGET Pathfinding_Bench_Program POST_BUILD
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} > /dev/null 2>&1
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
    )

    find_program(MAKE_EXECUTABLE make)
    if (MAKE_EXECUTABLE)
        add_custom_command(TARGET Pathfinding_Bench_Program POST_BUILD
                COMMAND ${MAKE_EXECUTABLE} > /dev/null 2>&1
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs/latex
                COMMENT "Generating PDF documentation"
        )
    else (MAKE_EXECUTABLE)
        message("Could not find 'make' command for generating PDF documentation.")
    endif ()
else (DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# set up testing
find_package(GTest)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Tests/UnitTests")

    # A-Star unit-Tests
    add_executable(AStarPathfinder_UnitTest Tests/UnitTests/a_star_unit_test.cpp)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Tests/UnitTests/astar_testgrid.bin
            ${CMAKE_CURRENT_BINARY_DIR}/Tests/UnitTests COPYONLY)
    target_link_libraries(AStarPathfinder_UnitTest gtest_main gtest pthread GridGenerator Pathfinder)
    add_test(NAME AStarUnitTest
            COMMAND AStarPathfinder_UnitTest
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Tests/UnitTests)

    # Dijsktra unit-Tests
    add_executable(DijkstraPathfinder_UnitTest Tests/UnitTests/dijkstra_unit_test.cpp)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Tests/UnitTests/dijkstra_testgrid.bin
            ${CMAKE_CURRENT_BINARY_DIR}/Tests/UnitTests COPYONLY)
    target_link_libraries(DijkstraPathfinder_UnitTest gtest_main gtest pthread GridGenerator Pathfinder)
    add_test(NAME DijkstraUnitTest
            COMMAND DijkstraPathfinder_UnitTest
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Tests/UnitTests)

else (GTEST_FOUND)
    message("Could not find gtest, skipping tests.")
endif (GTEST_FOUND)



