#include <gtest/gtest.h>

#include "pathfinding.h"
#include "grid.h"

TEST(Pathfinder, DijkstraUnitTest){
    auto finishedGrid = GridGenerator::Grid::deserialize("./dijkstra_testgrid.bin");
    auto gridToSolve = finishedGrid;
    gridToSolve.resetGrid();

    Pathfinder::DijkstraSolve solver{gridToSolve};
    solver.solveNoWait();

    EXPECT_EQ(finishedGrid.getClosedCellCount(), gridToSolve.getClosedCellCount());
    EXPECT_EQ(finishedGrid.getVisitedCellCount(), gridToSolve.getVisitedCellCount());
    EXPECT_EQ(finishedGrid.getPathCellCount(), gridToSolve.getPathCellCount());
}
