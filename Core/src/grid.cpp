#include "../inc/grid.h"

void GridGenerator::Grid::calculateCosts() {
    for (auto& row : cells){
        for(auto& cell: row){
            if(cell.getState() != CellState::CELL_OBSTACLE){
                cell.setGCost(calculateGCost(&cell));
                cell.setHCost(calculateHCost(&cell));
            }
        }
    }
}
