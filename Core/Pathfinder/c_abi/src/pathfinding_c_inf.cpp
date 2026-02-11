#include "pathfinding_c_inf.h"

#include <atomic>

static std::atomic<push_fn_t> registered_push{nullptr};

extern "C" void register_queue_push(push_fn_t f)
{
    registered_push.store(f, std::memory_order_release);
}

extern "C" void push_to_cell_queue(void *queue_handle, long x, long y)
{
    auto f = registered_push.load(std::memory_order_acquire);
    if (f)
    {
        f(queue_handle, x, y);
    }
}

neigbors_array_t get_neighbors_from_coord(grid_handle_t grid_handle, long x, long y)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    GridGenerator::GridCoordinate gc{x, y};
    auto neighbors = grid.getNeighborsCoordinates(gc);
    neigbors_array_t result;
    size_t i = 0;
    for (const auto &neighbor : neighbors)
    {
        result.coords[i].x = neighbor.x;
        result.coords[i].y = neighbor.y;
        i++;
    }
    for (; i < 8; i++)
    {
        result.coords[i].x = -1;
        result.coords[i].y = -1;
    }
    return result;
}

cost_struct_t get_cell_cost(grid_handle_t grid_handle, long x, long y)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    GridGenerator::GridCoordinate gc{x, y};
    auto &cell = grid(gc);
    cost_struct_t result;
    result.gCost = cell.getCost().gCost;
    result.hCost = cell.getCost().hCost;
    return result;
}

Cell_State get_cell_state(grid_handle_t grid_handle, long x, long y)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    GridGenerator::GridCoordinate gc{x, y};
    auto &cell = grid(gc);
    return static_cast<Cell_State>(cell.getState());
}

void increment_visited_cell_count(grid_handle_t grid_handle)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    grid.incrementVisitedCellCount();
}

void mark_cell_visited(grid_handle_t grid_handle, long x, long y)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    GridGenerator::GridCoordinate gc{x, y};
    auto &cell = grid(gc);
    cell.markVisited();
}

void set_cell_costs(grid_handle_t grid_handle, long x, long y, double gCost, double hCost)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    GridGenerator::GridCoordinate gc{x, y};
    auto &cell = grid(gc);
    cell.setGCost(gCost);
    cell.setHCost(hCost);
}

void set_parent_cell_pointer(grid_handle_t grid_handle, long x, long y,
                             long parentX, long parentY)
{
    auto &grid = *reinterpret_cast<GridGenerator::Grid *>(grid_handle);
    GridGenerator::GridCoordinate gc{x, y};
    GridGenerator::GridCoordinate parent_gc{parentX, parentY};
    auto &cell = grid(gc);
    cell.setParentCellPointer(&grid(parent_gc));
}