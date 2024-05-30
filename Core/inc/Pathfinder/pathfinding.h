#pragma once

#include "GridGenerator/grid.h"
#include <vector>
#include <map>

namespace Pathfinder {

    enum class PathfinderStrategy {
        PATHFINDER_DIJKSTRA = 1,
        PATHFINDER_A_STAR = 2,
    };

    static const std::map<PathfinderStrategy, std::string> pathfindingStrategyToDisplayableText =
            {{PathfinderStrategy::PATHFINDER_DIJKSTRA, "Dijkstra's Algorithm"},
             {PathfinderStrategy::PATHFINDER_A_STAR,   "A* Algorithm"}};

    class pathfindingParent {
    public:
        explicit pathfindingParent(GridGenerator::Grid &grid) : grid(grid) {}

        virtual void markShortestPath() = 0;

        virtual inline std::map<GridGenerator::Cell *, GridGenerator::Cell *>
        getShortestPath() { return shortestPath; };

    protected:
        std::map<GridGenerator::Cell *, GridGenerator::Cell *> shortestPath;
        GridGenerator::Grid &grid;

    };

    class DijkstraSolve : public pathfindingParent {
    public:
        void markShortestPath() override;
    };

    class AStarSolve : public pathfindingParent {
    public:
        void markShortestPath() override;
    };

}