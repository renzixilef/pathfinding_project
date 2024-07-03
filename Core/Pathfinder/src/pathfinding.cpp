#include "pathfinding.h"

const std::map<Pathfinder::PathfinderStrategy, std::string>
        Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText =
        {{PathfinderStrategy::PATHFINDER_DIJKSTRA, "Dijkstra's Algorithm"},
         {PathfinderStrategy::PATHFINDER_A_STAR,   "A* Algorithm"},
         {PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH, "Jump Point Search"}};

std::unique_ptr<Pathfinder::PathfindingParent> Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(
        PathfinderStrategy strat,
        GridGenerator::Grid &grid) {
    switch(strat){
        case PathfinderStrategy::PATHFINDER_A_STAR:
            return std::make_unique<AStarSolve>(grid);
        case PathfinderStrategy::PATHFINDER_DIJKSTRA:
            return std::make_unique<DijkstraSolve>(grid);
        case PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH:
            return std::make_unique<JumpPointSolve>(grid);
    }
    return nullptr;
}

void Pathfinder::PathfindingParent::initGenericSolver() {
    GridGenerator::GridCoordinate currentCoordinates = grid.getStartCoordinates();
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    grid(currentCoordinates).setGCost(0);
    grid(currentCoordinates).setHCost(currentCoordinates.getAbsDistanceTo(endCoordinates));
    grid(endCoordinates).setHCost(0);

    nextCellQueue.push(currentCoordinates);

}

bool Pathfinder::PathfindingParent::isCellBlockedOrOutOfBounds(int64_t x, int64_t y) {
    if(grid.isInBounds(x,y)){
        auto castX = static_cast<uint32_t>(x);
        auto castY = static_cast<uint32_t>(y);
        GridGenerator::CellState cellState = grid(GridGenerator::GridCoordinate{castX,castY}).getState();
        return cellState == GridGenerator::CellState::CELL_OBSTACLE;
    }
    return true;
}

void Pathfinder::PathfindingParent::solveNoWait() {
    while(grid.getStatus() == GridGenerator::GridSolvedStatus::GRID_UNSOLVED){
        if (!queueEmpty()) {
            nextStep();
        } else {
            grid.setUnsolvable();
        }
        if (grid.getEndCell()->getState() == GridGenerator::CellState::CELL_PATH) {
            grid.setSolved();
        }
    }
}

void Pathfinder::PathfindingParent::reInit() {
    nextCellQueue.pop();
    grid.resetGrid();
    initGenericSolver();
}
