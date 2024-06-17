#pragma once

#include "GridGenerator/grid.h"
#include "timer.h"

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <queue>
#include <optional>

//TODO: implement functionality to get private grid information

namespace Pathfinder {

    enum class PathfinderStrategy {
        PATHFINDER_DIJKSTRA = 1,
        PATHFINDER_A_STAR = 2,
        PATHFINDER_JUMP_POINT_SEARCH = 3
    };
    struct PathfinderPerformanceMetric {
        uint32_t pathCells;
        uint32_t visitedCells;
        uint32_t closedCells;
        double solvingSeconds;
        double avgSeconsPerStep;
        uint32_t stepCount;
        PathfinderStrategy strat;

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

        inline PathfinderPerformanceMetric getPerformanceMetric() {
            return PathfinderPerformanceMetric{grid.getPathCells(),
                                               grid.getVisitedCells(),
                                               grid.getClosedCells(),
                                               timer.getAbsoluteTime(),
                                               timer.getAvgUSecondsPerStep(),
                                               timer.getStepCount(),
                                               strat};
        }


    protected:
        PathfinderStrategy strat;

        PathfinderTimer timer;

        virtual void setStrat() = 0;

        GridGenerator::Grid &grid;
        std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
                decltype(grid.compareCells())> nextCellQueue{grid.compareCells()};

        bool isCellBlockedOrOutOfBounds(int64_t x, int64_t y);

    private:
        void initGenericSolver();
    };

    class DijkstraSolve : public pathfindingParent {
    public:
        explicit DijkstraSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) { setStrat(); }

        void nextStep() override;

    private:
        inline void setStrat() override { strat = PathfinderStrategy::PATHFINDER_DIJKSTRA; }
    };

    class AStarSolve : public pathfindingParent {
    public:
        explicit AStarSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) { setStrat(); }

        void nextStep() override;

    private:
        inline void setStrat() override { strat = PathfinderStrategy::PATHFINDER_A_STAR; }
    };

    class JumpPointSolve : public pathfindingParent {
    public:
        explicit JumpPointSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {
            initJPSSolver();
            setStrat();
        }

        void nextStep() override;

    private:
        inline void setStrat() override { strat = PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH; }

        std::optional<GridGenerator::GridCoordinate> jump(GridGenerator::GridCoordinate currentCoord,
                                                          std::pair<int8_t, int8_t> direction);

        std::unordered_map<GridGenerator::GridCoordinate,
                std::vector<std::pair<int8_t, int8_t>>, decltype(&GridGenerator::GridCoordinate::getHash)>
                directionMap{0, GridGenerator::GridCoordinate::getHash};

        void initJPSSolver();

    };

}