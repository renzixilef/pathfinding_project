#include "Pathfinder/pathfinding.h"

#include <iostream>
#include <queue>

void Pathfinder::AStarSolve::markShortestPath() {
    triggerCallbacks(Pathfinder::CallbackType::CALLBACK_BEFORE_RUN);
    GridGenerator::GridCoordinate currentCoordinates = grid.getStartCoordinates();
    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();
    std::vector<GridGenerator::GridCoordinate> neighbors;

    grid(currentCoordinates).setGCost(0);
    grid(currentCoordinates).setHCost(currentCoordinates.getAbsDistanceTo(endCoordinates));
    grid(endCoordinates).setHCost(0);

    std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
            decltype(grid.compareCells())> nextCellQueue(grid.compareCells());
    nextCellQueue.push(currentCoordinates);

    while (!nextCellQueue.empty()) {
        triggerCallbacks(Pathfinder::CallbackType::CALLBACK_BEFORE_STEP);
        currentCoordinates = nextCellQueue.top();
        nextCellQueue.pop();
        //TODO: this line modifies the grid, why?
        GridGenerator::Cell &currentCell = grid(currentCoordinates);
        if(currentCell.getState() == GridGenerator::CellState::CELL_CLOSED) continue;
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
        triggerCallbacks(Pathfinder::CallbackType::CALLBACK_AFTER_STEP);
    }
    if(grid(endCoordinates).getState() == GridGenerator::CellState::CELL_PATH){
        grid.setSolved();
        std::cout << "Done! Solved" << std::endl;
    }else{
        grid.setUnsolvable();
    }
    triggerCallbacks(Pathfinder::CallbackType::CALLBACK_AFTER_RUN);
}