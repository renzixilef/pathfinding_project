#pragma once

//TODO: documentation

namespace GridGenerator {

    enum class CellState {
        CELL_OBSTACLE = false,
        CELL_OPEN = 0x1,
        CELL_VISITED = 0x2,
        CELL_PATH = 0x3
    };

    struct CellCost {
        float gCost;
        float hCost;

        [[nodiscard]] inline float totalCost() const { return gCost + hCost; }
    };


    class Cell {
    public:
        Cell() : state(CellState::CELL_OPEN), cost(CellCost{}), parent(nullptr) {}

        inline void markObstacle() { state = CellState::CELL_OBSTACLE; }

        inline void markOpen() { state = CellState::CELL_OPEN; }

        inline void markVisited() { state = CellState::CELL_VISITED; }

        inline void markPath() { state = CellState::CELL_PATH; }

        inline void setGCost(float gCost) { cost.gCost = gCost; }

        inline void setHCost(float hCost) { cost.hCost = hCost; }

        inline void setParent(Cell *parentCell) { parent = parentCell; }

        [[nodiscard]] inline CellState getState() const { return state; }

        [[nodiscard]] inline CellCost getCost() const { return cost; }

    private:
        CellState state;
        CellCost cost;
        Cell *parent;
    };
}