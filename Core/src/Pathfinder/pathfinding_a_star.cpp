#include "Pathfinder/pathfinding.h"

#include <iostream>
#include <queue>

void Pathfinder::AStarSolve::nextStep() {
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    GridGenerator::GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();

    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    if (currentCell.getState() == GridGenerator::CellState::CELL_CLOSED) return;
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
            neighborCell.setHCost(neighborCoordinates.getAbsDistanceTo(endCoordinates));
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
        std::cout << "Step done" << std::endl;
    }
    currentCell.markClosed();
    grid.incrementClosedCellCount();
}

