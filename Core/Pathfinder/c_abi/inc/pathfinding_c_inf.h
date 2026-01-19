#pragma once

#include "grid.h"
#include <queue>

extern "C" {
    using grid_handle_t = void*;
    using cell_queue_handle_t = void*;

    typedef struct {
        long x;
        long y;
    } grid_coordinate_struct_t;

    typedef struct {
        grid_coordinate_struct_t coords[8];
    } neigbors_array_t;

    typedef struct {
        double gCost;
        double hCost;
    } cost_struct_t;

    enum Cell_State {
        CELL_OBSTACLE = 0, /**< Represents cells that are obstacles. */
        CELL_OPEN = 1, /**< Represents open cells. */
        CELL_VISITED = 2, /**< Represents cells that have been visited. */
        CELL_CLOSED = 3, /**< Represents cells that have been closed. */
        CELL_PATH = 4 /**< Represents cells that are part of the path. */
    };

    typedef void (*push_fn_t)(void *queue_handle, long x, long y);

    void register_queue_push(push_fn_t f);

    void push_to_cell_queue(void *queue_handle, long x, long y);

    neigbors_array_t get_neighbors_from_coord(grid_handle_t grid_handle, long x, long y);

    cost_struct_t get_cell_cost(grid_handle_t grid_handle, long x, long y);

    Cell_State get_cell_state(grid_handle_t grid_handle, long x, long y);

    void increment_visited_cell_count(grid_handle_t grid_handle);

    void mark_cell_visited(grid_handle_t grid_handle, long x, long y);

    void set_cell_costs(grid_handle_t grid_handle, long x, long y, double gCost, double hCost);

    void set_parent_cell_pointer(grid_handle_t grid_handle, long x, long y,
                                 long parentX, long parentY);
}