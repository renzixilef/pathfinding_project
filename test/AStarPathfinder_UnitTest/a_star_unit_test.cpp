#include <gtest/gtest.h>
#include <fstream>

#include "Pathfinder/pathfinding.h"

TEST(AStarPathfinder, UnitTest){
    auto finishedGrid = GridGenerator::Grid::deserialize("astar_testgrid.bin");
    auto gridToSolve = finishedGrid;
    gridToSolve.resetGrid();

    Pathfinder::AStarSolve solver{gridToSolve};
    solver.solveNoWait();


}
