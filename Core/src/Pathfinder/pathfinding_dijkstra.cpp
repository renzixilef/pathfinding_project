#include "Pathfinder/pathfinding.h"

#include <queue>

void Pathfinder::DijkstraSolve::markShortestPath() {
    GridGenerator::GridCoordinate currentCoordinates = grid.getStartCoordinates();
    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();
    std::vector<GridGenerator::GridCoordinate> neighbors;

    grid(currentCoordinates).setGCost(0);

    std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
            decltype(grid.compareCells())> nextCellQueue(grid.compareCells());
    nextCellQueue.push(currentCoordinates);

    while (!nextCellQueue.empty()) {
        currentCoordinates = nextCellQueue.top();
        nextCellQueue.pop();
        GridGenerator::Cell &currentCell = grid(currentCoordinates);
        if(currentCoordinates == endCoordinates){
            grid.markPathByParentCells();
            break;
        }

        neighbors = grid.getNeighborsCoordinates(currentCoordinates);

        for (const auto &neighborCoordinates: neighbors) {
            GridGenerator::Cell &neighborCell = grid(neighborCoordinates);
            double neighborCellGCostFromCurrentCell = currentCell.getCost().gCost +
                                                         neighborCoordinates.getAbsDistanceTo(currentCoordinates);
            if (neighborCell.getState() == GridGenerator::CellState::CELL_OPEN) {
                neighborCell.setGCost(neighborCellGCostFromCurrentCell);
                neighborCell.setParent(&grid(currentCoordinates));
                neighborCell.markVisited();
                nextCellQueue.push(neighborCoordinates);
            } else if (neighborCell.getState() == GridGenerator::CellState::CELL_VISITED) {
                if (neighborCell.getCost().gCost > neighborCellGCostFromCurrentCell) {
                    neighborCell.setGCost(neighborCellGCostFromCurrentCell);
                    neighborCell.setParent(&grid(currentCoordinates));
                    nextCellQueue.push(neighborCoordinates);
                }
            }
        }
        currentCell.markClosed();
    }
    if(grid(endCoordinates).getState() == GridGenerator::CellState::CELL_PATH){
        grid.setSolved();
    }else{
        grid.setUnsolvable();
    }
}


