#include <gtest/gtest.h>
#include <fstream>
#include <bitset>
#include "pathfinding.h"
#include "grid.h"

TEST(Pathfinder, AStarUnitTest){
    auto finishedGrid = GridGenerator::Grid::deserialize("./astar_testgrid.bin");
    auto gridToSolve = finishedGrid;

    gridToSolve.resetGrid();

    Pathfinder::AStarSolve solver = Pathfinder::AStarSolve(gridToSolve);
    solver.solveNoWait();

    EXPECT_EQ(finishedGrid.getClosedCellCount(), gridToSolve.getClosedCellCount());
    EXPECT_EQ(finishedGrid.getVisitedCellCount(), gridToSolve.getVisitedCellCount());
    EXPECT_EQ(finishedGrid.getPathCellCount(), gridToSolve.getPathCellCount());
}
