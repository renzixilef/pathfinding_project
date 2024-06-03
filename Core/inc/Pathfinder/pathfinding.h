#pragma once

#include "GridGenerator/grid.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <queue>

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
        explicit pathfindingParent(GridGenerator::Grid &grid) : grid(grid), nextCellQueue(grid.compareCells()) {
            initSolver();
        }
        virtual ~pathfindingParent() = default;
        inline bool queueEmpty(){return nextCellQueue.empty();}
        virtual void nextStep() = 0;
    protected:
        GridGenerator::Grid& grid;
        std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
                decltype(grid.compareCells())> nextCellQueue;
    private:
        void initSolver();
    };

    class DijkstraSolve : public pathfindingParent {
    public:
        explicit DijkstraSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        void nextStep() override;
    };

    class AStarSolve : public pathfindingParent {
    public:
        explicit AStarSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        void nextStep() override;
    };

}