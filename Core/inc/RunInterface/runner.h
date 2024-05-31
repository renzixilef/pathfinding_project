#pragma once

#include <cstdint>
#include <list>

#include "../GridGenerator/obstacle_gen.h"
#include "../Pathfinder/pathfinding.h"


//TODO: implement register Callback functionality
//TODO: implement iterative solving for multiSolver

namespace RunInterface {

    struct RunGridConfig {
        uint32_t gridWith;
        uint32_t gridHeight;
        float obstacleDensity;
        float minStartEndDistance;
        GridGenerator::ObstacleGenStrategy obstacleGenStrategy;

    };


    class RunnerParent {
    public:
        explicit RunnerParent(const RunGridConfig &thisConfig);

        virtual void start() = 0;

    protected:
        RunGridConfig config;
        GridGenerator::Grid grid;

    };

    class SingleRun : public RunnerParent {
    public:
        explicit SingleRun(const RunGridConfig &thisConfig, const Pathfinder::PathfinderStrategy &thisStrat);

        void start() override;

    private:
        Pathfinder::PathfinderStrategy strat;
        std::unique_ptr<Pathfinder::pathfindingParent> solver;
    };

    class MultiRun : public RunnerParent {
    public:
        explicit MultiRun(const RunGridConfig &thisConfig,
                          const std::list<Pathfinder::PathfinderStrategy> &thisStrats,
                          uint32_t thisIterations);

        void start() override;

    private:
        std::list<Pathfinder::PathfinderStrategy> strats;
        uint32_t iterations;
        std::list<std::unique_ptr<Pathfinder::pathfindingParent>> solvers;
    };
}