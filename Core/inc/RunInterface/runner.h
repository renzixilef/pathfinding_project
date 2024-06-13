#pragma once

#include <cstdint>
#include <list>
#include <QObject>

#include "../GridGenerator/obstacle_gen.h"
#include "../Pathfinder/pathfinding.h"


//TODO: implement register Callback functionality
//TODO: implement iterative solving for multiSolver

namespace RunInterface {

    struct RunGridConfig {
        uint32_t gridWidth{};
        uint32_t gridHeight{};
        float obstacleDensity{};
        float minStartEndDistance{};
        GridGenerator::ObstacleGenStrategy obstacleGenStrategy{GridGenerator::ObstacleGenStrategy::OBSTACLE_RANDOM};
        std::optional<uint32_t> iterations = std::nullopt;
    };


    class RunnerParent : public QObject {
    Q_OBJECT
    public:
        explicit RunnerParent(const RunGridConfig &thisConfig);

        [[nodiscard]] const inline GridGenerator::Grid &getGridRef() const { return grid; }

    signals:

        void stepFinished();

        void gridFinished();

    public slots:

        virtual void nextStep() = 0;

    protected:
        RunGridConfig config;
        GridGenerator::Grid grid;


    };

    class SingleRun : public RunnerParent {
    Q_OBJECT
    public:
        explicit SingleRun(const RunGridConfig &thisConfig, const Pathfinder::PathfinderStrategy &thisStrat);

        void nextStep() override;

    public slots:

        void onRunReset();

    private:
        Pathfinder::PathfinderStrategy strat;
        std::unique_ptr<Pathfinder::pathfindingParent> solver;
    };

    class MultiRun : public RunnerParent {
    public:
        explicit MultiRun(const RunGridConfig &thisConfig,
                          const std::list<Pathfinder::PathfinderStrategy> &thisStrats);

        void nextStep() override;

    private:
        std::list<Pathfinder::PathfinderStrategy> strats;
        uint32_t iterations;
        std::list<std::unique_ptr<Pathfinder::pathfindingParent>> solvers;
    };
}
