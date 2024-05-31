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
            return std::make_unique<DijkstraSolve>(grid);
        case PathfinderStrategy::PATHFINDER_DIJKSTRA:
            return std::make_unique<AStarSolve>(grid);
    }
    return nullptr;
}