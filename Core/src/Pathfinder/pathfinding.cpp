#include "../../inc/Pathfinder/pathfinding.h"

const std::map<Pathfinder::PathfinderStrategy, std::string>
        Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText =
        {{PathfinderStrategy::PATHFINDER_DIJKSTRA, "Dijkstra's Algorithm"},
         {PathfinderStrategy::PATHFINDER_A_STAR,   "A* Algorithm"},
         {PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH, "Jump Point Search"}};

std::unique_ptr<Pathfinder::pathfindingParent> Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(
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

void Pathfinder::pathfindingParent::initGenericSolver() {
    GridGenerator::GridCoordinate currentCoordinates = grid.getStartCoordinates();
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    grid(currentCoordinates).setGCost(0);
    grid(currentCoordinates).setHCost(currentCoordinates.getAbsDistanceTo(endCoordinates));
    grid(endCoordinates).setHCost(0);

    nextCellQueue.push(currentCoordinates);

}

bool Pathfinder::pathfindingParent::isCellBlockedOrOutOfBounds(int64_t x, int64_t y) {
    if(grid.isInBounds(x,y)){
        auto castX = static_cast<uint32_t>(x);
        auto castY = static_cast<uint32_t>(y);
        GridGenerator::CellState cellState = grid(GridGenerator::GridCoordinate{castX,castY}).getState();
        return cellState == GridGenerator::CellState::CELL_OBSTACLE;
    }
    return true;
}
