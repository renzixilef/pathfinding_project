#include "pathfinding.h"
#include "pathfinding_c_inf.h"

extern "C" void ada_dijkstra_next_step(void *grid_handle, long currentX, long currentY, void *queue_handle);
extern "C" void pathfinder_push_impl(void *queue_handle, long x, long y);

Pathfinder::DijkstraSolveAda::DijkstraSolveAda(GridGenerator::Grid &grid) : PathfindingParent(grid)
{
    register_queue_push(pathfinder_push_impl);
}

void Pathfinder::DijkstraSolveAda::nextStep()
{
    // TODO: implement Ada backend call and translate for Ada interfacing
    timer.stepBegin();
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    GridGenerator::GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();
    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    if (currentCoordinates == endCoordinates)
    {
        timer.stepEnd();
        grid.markPathByParentCells(true);
        return;
    }

    ada_dijkstra_next_step(reinterpret_cast<void *>(&grid),
                           static_cast<long>(currentCoordinates.x),
                           static_cast<long>(currentCoordinates.y),
                           reinterpret_cast<void *>(&nextCellQueue));

    currentCell.markClosed();
    grid.incrementClosedCellCount();
    timer.stepEnd();
}
