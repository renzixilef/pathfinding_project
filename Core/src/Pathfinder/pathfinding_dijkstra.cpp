#include "Pathfinder/pathfinding.h"

#include <queue>

void Pathfinder::DijkstraSolve::nextStep() {
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    GridGenerator::GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();
    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    if (currentCoordinates == endCoordinates) {
        grid.markPathByParentCells();
        return;
    }

    std::vector<GridGenerator::GridCoordinate> neighbors = grid.getNeighborsCoordinates(currentCoordinates);

    for (const auto &neighborCoordinates: neighbors) {
        GridGenerator::Cell &neighborCell = grid(neighborCoordinates);
        double neighborCellGCostFromCurrentCell = currentCell.getCost().gCost +
                                                  neighborCoordinates.getAbsDistanceTo(currentCoordinates);
        if (neighborCell.getState() == GridGenerator::CellState::CELL_OPEN) {
            neighborCell.setGCost(neighborCellGCostFromCurrentCell);
            neighborCell.setParent(&grid(currentCoordinates));
            neighborCell.markVisited();
            nextCellQueue.push(neighborCoordinates);
            grid.incrementVisitCount();
        } else if (neighborCell.getState() == GridGenerator::CellState::CELL_VISITED) {
            if (neighborCell.getCost().gCost > neighborCellGCostFromCurrentCell) {
                neighborCell.setGCost(neighborCellGCostFromCurrentCell);
                neighborCell.setParent(&grid(currentCoordinates));
                nextCellQueue.push(neighborCoordinates);
                grid.incrementVisitCount();
            }
        }
    }
    currentCell.markClosed();
    grid.incrementClosedCellCount();
}


