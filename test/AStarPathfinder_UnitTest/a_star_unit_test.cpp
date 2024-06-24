#include <gtest/gtest.h>
#include <fstream>

#include "pathfinding.h"
#include "grid.h"

TEST(Pathfinder, AStarUnitTest){
    auto finishedGrid = GridGenerator::Grid::deserialize("astar_testgrid.bin");
    auto gridToSolve = finishedGrid;
    gridToSolve.resetGrid();

    Pathfinder::AStarSolve solver{gridToSolve};
    solver.solveNoWait();

    for(uint32_t x = 0; x<finishedGrid.getSizeX(); x++){
        for(uint32_t y = 0; y<finishedGrid.getSizeY(); y++){
            GridGenerator::GridCoordinate coord{x,y};
            EXPECT_EQ(finishedGrid(coord).getState(), gridToSolve(coord).getState());
        }
    }
}
