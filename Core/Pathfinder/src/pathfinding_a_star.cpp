#include "pathfinding.h"

#include <queue>

void Pathfinder::AStarSolve::nextStep() {
    timer.stepBegin();
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    GridGenerator::GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();

    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    if (currentCell.getState() == GridGenerator::CellState::CELL_CLOSED) return;
    if (currentCoordinates == endCoordinates) {
        grid.markPathByParentCells(true);
        return;
    }


    std::vector<GridGenerator::GridCoordinate> neighbors = grid.getNeighborsCoordinates(currentCoordinates);

    for (const auto &neighborCoordinates: neighbors) {
        GridGenerator::Cell &neighborCell = grid(neighborCoordinates);
        double neighborCellGCostFromCurrentCell = currentCell.getCost().gCost +
                                                  neighborCoordinates.getOctileDistanceTo(currentCoordinates);

        if (neighborCell.getState() == GridGenerator::CellState::CELL_OPEN) {
            neighborCell.setGCost(neighborCellGCostFromCurrentCell);
            neighborCell.setHCost(neighborCoordinates.getAbsDistanceTo(endCoordinates));
            neighborCell.setParentCellPointer(&grid(currentCoordinates));
            neighborCell.markVisited();
            nextCellQueue.push(neighborCoordinates);
            grid.incrementVisitedCellCount();
        } else if (neighborCell.getState() == GridGenerator::CellState::CELL_VISITED) {
            if (neighborCell.getCost().gCost > neighborCellGCostFromCurrentCell) {
                neighborCell.setGCost(neighborCellGCostFromCurrentCell);
                neighborCell.setParentCellPointer(&grid(currentCoordinates));
                nextCellQueue.push(neighborCoordinates);
                grid.incrementVisitedCellCount();
            }
        }
    }
    currentCell.markClosed();
    grid.incrementClosedCellCount();
    timer.stepEnd();
}

