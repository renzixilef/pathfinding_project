#include "../inc/pathfinding.h"

#include <queue>

void Pathfinder::AStarSolve::markShortestPath() {
    GridGenerator::GridCoordinates currentCoordinates = grid.getStartCoordinates();
    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    GridGenerator::GridCoordinates endCoordinates = grid.getEndCoordinates();
    std::vector<GridGenerator::GridCoordinates> neighbors;

    grid(currentCoordinates).setGCost(0);
    grid(currentCoordinates).setHCost(currentCoordinates.getAbsDistanceTo(endCoordinates));
    grid(currentCoordinates).markPath();
    grid(endCoordinates).setHCost(0);

    std::priority_queue<GridGenerator::GridCoordinates, std::vector<GridGenerator::GridCoordinates>,
            decltype(grid.compareCells())> nextCellQueue(grid.compareCells());
    nextCellQueue.push(currentCoordinates);

    while (!nextCellQueue.empty()) {
        currentCoordinates = nextCellQueue.top();
        nextCellQueue.pop();
        currentCell = grid(currentCoordinates);

        neighbors = grid.getNeighborsCoordinates(currentCoordinates);

        for (const auto &neighborCoordinates: neighbors) {
            GridGenerator::Cell &neighborCell = grid(neighborCoordinates);
            float neighborCellTotalCostFromCurrentCell = currentCell.getCost().totalCost() +
                                                         neighborCoordinates.getAbsDistanceTo(currentCoordinates);
            if (neighborCoordinates == endCoordinates) {
                grid.getEndCell()->setGCost(neighborCellTotalCostFromCurrentCell);
                grid.setSolved();
                grid.markPathByParentCells();
                return;
            } else if (neighborCell.getState() == GridGenerator::CellState::CELL_OPEN) {
                neighborCell.setGCost(neighborCellTotalCostFromCurrentCell);
                neighborCell.setHCost(neighborCoordinates.getAbsDistanceTo(endCoordinates));
                neighborCell.setParent(&grid(currentCoordinates));
                neighborCell.markVisited();
                nextCellQueue.push(neighborCoordinates);
            } else if (neighborCell.getState() == GridGenerator::CellState::CELL_VISITED) {
                if (neighborCell.getCost().totalCost() > neighborCellTotalCostFromCurrentCell) {
                    neighborCell.setGCost(neighborCellTotalCostFromCurrentCell);
                    neighborCell.setParent(&grid(currentCoordinates));
                    nextCellQueue.push(neighborCoordinates);
                }
            }
        }
        grid.setUnsolvable();
    }

}