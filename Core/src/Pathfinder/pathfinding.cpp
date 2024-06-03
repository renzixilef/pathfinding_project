#include "../../inc/Pathfinder/pathfinding.h"

const std::map<Pathfinder::PathfinderStrategy, std::string>
        Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText =
        {{PathfinderStrategy::PATHFINDER_DIJKSTRA, "Dijkstra's Algorithm"},
         {PathfinderStrategy::PATHFINDER_A_STAR,   "A* Algorithm"}};

std::unique_ptr<Pathfinder::pathfindingParent> Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(
        PathfinderStrategy strat,
        GridGenerator::Grid &grid) {
    switch(strat){
        case PathfinderStrategy::PATHFINDER_A_STAR:
            return std::make_unique<AStarSolve>(grid);
        case PathfinderStrategy::PATHFINDER_DIJKSTRA:
            return std::make_unique<DijkstraSolve>(grid);
    }
    return nullptr;
}

void Pathfinder::pathfindingParent::initSolver() {
    GridGenerator::GridCoordinate currentCoordinates = grid.getStartCoordinates();
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();

    grid(currentCoordinates).setGCost(0);
    grid(currentCoordinates).setHCost(currentCoordinates.getAbsDistanceTo(endCoordinates));
    grid(endCoordinates).setHCost(0);

    nextCellQueue.push(currentCoordinates);

}
