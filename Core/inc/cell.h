#pragma once

//TODO: documentation

namespace GridGenerator {

    enum class CellState {
        CELL_OBSTACLE = false,
        CELL_OPEN = 0x1,
        CELL_VISITED = 0x2,
        CELL_PATH = 0x3
    };

    struct CellCost{
        float gCost;
        float hCost;
    };


    class Cell {
    public:
        Cell(): state(CellState::CELL_OPEN), cost(CellCost{}){}
        inline void markObstacle(){state = CellState::CELL_OBSTACLE;}
        inline void markOpen(){state = CellState::CELL_OPEN;}
        inline void markVisited(){state = CellState::CELL_VISITED;}
        inline void markPATH(){state = CellState::CELL_PATH;}
        inline CellState getState(){return state;}
        inline CellCost getCost(){return cost;}

    private:
        CellState state;
        CellCost cost;
    };
}