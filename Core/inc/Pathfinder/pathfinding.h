#pragma once

#include "GridGenerator/grid.h"
#include <vector>
#include <map>
#include <memory>
#include <functional>

//TODO: implement functionality to get private grid information
//TODO: implement gridReset to solve nextGrid

namespace Pathfinder {

    enum class CallbackType{
        CALLBACK_AFTER_STEP,
        CALLBACK_AFTER_RUN,
        CALLBACK_BEFORE_STEP,
        CALLBACK_BEFORE_RUN
    };

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
        void registerCallback(CallbackType callbackType, const std::function<void()>& callback);
        void triggerCallbacks(CallbackType callbackType);
    protected:
        GridGenerator::Grid& grid;
        std::vector<std::function<void()>> afterStepCallback;
        std::vector<std::function<void()>> afterRunCallback;
        std::vector<std::function<void()>> beforeStepCallback;
        std::vector<std::function<void()>> beforeRunCallback;
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