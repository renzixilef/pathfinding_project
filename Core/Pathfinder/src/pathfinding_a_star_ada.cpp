#include "pathfinding.h"
#include "pathfinding_c_inf.h"

extern "C" void ada_astar_next_step(void *grid_handle, grid_coordinate_struct_t current, grid_coordinate_struct_t end, void *queue_handle);
extern "C" void pathfinder_push_impl(void *queue_handle, long x, long y);

Pathfinder::AStarSolveAda::AStarSolveAda(GridGenerator::Grid &grid) : PathfindingParent(grid)
{
    register_queue_push(pathfinder_push_impl);
}

void Pathfinder::AStarSolveAda::nextStep()
{
    // TODO: implement Ada backend call and translate for Ada interfacing
    timer.stepBegin();
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    GridGenerator::GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();

    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    if (currentCell.getState() == GridGenerator::CellState::CELL_CLOSED)
        return;
    if (currentCoordinates == endCoordinates)
    {
        timer.stepEnd();
        grid.markPathByParentCells(true);
        return;
    }

    grid_coordinate_struct_t current = {static_cast<long>(currentCoordinates.x),
                                        static_cast<long>(currentCoordinates.y)};
    grid_coordinate_struct_t end = {static_cast<long>(endCoordinates.x),
                                    static_cast<long>(endCoordinates.y)};

    ada_astar_next_step(reinterpret_cast<void *>(&grid),
                        current,
                        end,
                        reinterpret_cast<void *>(&nextCellQueue));

    currentCell.markClosed();
    grid.incrementClosedCellCount();
    timer.stepEnd();
}
