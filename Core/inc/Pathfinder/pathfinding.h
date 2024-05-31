#pragma once

#include "GridGenerator/grid.h"
#include <vector>
#include <map>
#include <memory>

//TODO: implement functionality to get private grid information
//TODO: implement gridReset to solve nextGrid

namespace Pathfinder {

    enum class PathfinderStrategy {
        PATHFINDER_DIJKSTRA = 1,
        PATHFINDER_A_STAR = 2,
    };

    class pathfindingParent;

    struct PathfinderStrategyParser {
        static const std::map<PathfinderStrategy, std::string> pathfindingStrategyToDisplayableText;

        static std::unique_ptr<pathfindingParent> parsePathfinderStrategy(PathfinderStrategy strat,
                                                                   GridGenerator::Grid &grid);
    };


    class pathfindingParent {
    public:
        explicit pathfindingParent(GridGenerator::Grid &grid) : grid(grid) {}
        virtual ~pathfindingParent() = default;
        virtual void markShortestPath() = 0;

        virtual inline std::map<GridGenerator::Cell *, GridGenerator::Cell *>
        getShortestPath() { return shortestPath; };

    protected:
        std::map<GridGenerator::Cell *, GridGenerator::Cell *> shortestPath;
        GridGenerator::Grid grid;

    };

    class DijkstraSolve : public pathfindingParent {
    public:
        explicit DijkstraSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        void markShortestPath() override;
    };

    class AStarSolve : public pathfindingParent {
    public:
        explicit AStarSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        void markShortestPath() override;
    };

}