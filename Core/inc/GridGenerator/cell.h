#pragma once
#include <QColor>
#include <variant>


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
        using CellParentType = std::variant<Cell*, std::pair<int8_t, int8_t>>;
    public:
        Cell() : state(CellState::CELL_OPEN), cost(CellCost{}), parent(nullptr) {}

        inline void markObstacle() { state = CellState::CELL_OBSTACLE; }

        inline void markOpen() { state = CellState::CELL_OPEN; }

        inline void markVisited() { state = CellState::CELL_VISITED; }

        inline void markClosed() { state = CellState::CELL_CLOSED; }

        inline void markPath() { state = CellState::CELL_PATH; }

        inline void setGCost(double gCost) { cost.gCost = gCost; }

        inline void setHCost(double hCost) { cost.hCost = hCost; }

        inline void setParentCellPointer(Cell *parentCell) { parent = parentCell; }
        inline void setParentDirPair(std::pair<int8_t, int8_t> parentDir) { parent = parentDir; }

        inline Cell **getParentIfCellPointer() { return std::get_if<Cell*>(&parent); }

        inline std::pair<int8_t, int8_t>* getParentIfDirPair() {
            return std::get_if<std::pair<int8_t, int8_t>>(&parent);
        }

        [[nodiscard]] inline CellState getState() const { return state; }

        [[nodiscard]] inline CellCost getCost() const { return cost; }

    private:
        CellState state;
        CellCost cost;
        CellParentType parent;
    };
}