#pragma once

#include "GridGenerator/grid.h"
#include <vector>
#include <map>
#include <memory>
#include <optional>

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

    protected:
        GridGenerator::Grid& grid;

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

    class JumpPointSolve : public pathfindingParent{
    public:
        explicit JumpPointSolve(GridGenerator::Grid &grid) : pathfindingParent(grid){}

        void markShortestPath() override;
    private:
        std::optional<GridGenerator::GridCoordinate> getJumpPoint(GridGenerator::GridCoordinate currentCoord,
                                                   GridGenerator::GridCoordinate neighborCoord);

    };

}