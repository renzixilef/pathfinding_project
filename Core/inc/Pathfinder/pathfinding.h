#pragma once

#include "GridGenerator/grid.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <queue>
#include <optional>

//TODO: implement functionality to get private grid information
//TODO: implement gridReset to solve nextGrid

namespace Pathfinder {

    enum class PathfinderStrategy {
        PATHFINDER_DIJKSTRA = 1,
        PATHFINDER_A_STAR = 2,
        PATHFINDER_JUMP_POINT_SEARCH = 3
    };

    class pathfindingParent;

    struct PathfinderStrategyParser {
        static const std::map<PathfinderStrategy, std::string> pathfindingStrategyToDisplayableText;

        static std::unique_ptr<pathfindingParent> parsePathfinderStrategy(PathfinderStrategy strat,
                                                                          GridGenerator::Grid &grid);
    };


    class pathfindingParent {
    public:
        explicit pathfindingParent(GridGenerator::Grid &grid) :
                grid(grid) {
            initGenericSolver();
        }

        virtual ~pathfindingParent() = default;

        inline bool queueEmpty() { return nextCellQueue.empty(); }

        virtual void nextStep() = 0;

    protected:
        GridGenerator::Grid &grid;
        std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
                decltype(grid.compareCells())> nextCellQueue{grid.compareCells()};
    private:
        void initGenericSolver();
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

    class JumpPointSolve : public pathfindingParent {
    public:
        explicit JumpPointSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {
            initJPSSolver();
        }

        void nextStep() override;

    private:
        void addJumpPointsToQueue(GridGenerator::GridCoordinate currentCoord,
                                  std::pair<int8_t, int8_t> direction,
                                  GridGenerator::GridCoordinate basePoint,
                                  bool recursed = false);

        std::unordered_map<GridGenerator::GridCoordinate,
                std::vector<std::pair<int8_t, int8_t>>, decltype(&GridGenerator::GridCoordinate::getHash)>
                directionMap{0, GridGenerator::GridCoordinate::getHash};

        void initJPSSolver();

    };

}