#pragma once
#include <QColor>

//TODO: documentation

namespace GridGenerator {

    enum class CellState {
        CELL_OBSTACLE = 0,
        CELL_OPEN = 1,
        CELL_VISITED = 2,
        CELL_CLOSED = 3,
        CELL_PATH = 4
    };

    struct CellStateParser{
        static QColor parseCellStateToQColor(CellState state, bool&& startEnd = false);
    };

    struct CellCost {
        double gCost;
        double hCost;

        [[nodiscard]] inline double totalCost() const { return gCost + hCost; }
    };


    class Cell {
    public:
        Cell() : state(CellState::CELL_OPEN), cost(CellCost{}), parent(nullptr) {}

        inline void markObstacle() { state = CellState::CELL_OBSTACLE; }

        inline void markOpen() { state = CellState::CELL_OPEN; }

        inline void markVisited() { state = CellState::CELL_VISITED; }

        inline void markClosed() { state = CellState::CELL_CLOSED; }

        inline void markPath() { state = CellState::CELL_PATH; }

        inline void setGCost(double gCost) { cost.gCost = gCost; }

        inline void setHCost(double hCost) { cost.hCost = hCost; }

        inline void setParent(Cell *parentCell) { parent = parentCell; }

        inline Cell *getParent() { return parent; }

        [[nodiscard]] inline CellState getState() const { return state; }

        [[nodiscard]] inline CellCost getCost() const { return cost; }

    private:
        CellState state;
        CellCost cost;
        Cell *parent;
    };
}