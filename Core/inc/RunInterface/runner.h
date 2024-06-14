#pragma once

#include <cstdint>
#include <list>
#include <QObject>

#include "../GridGenerator/obstacle_gen.h"
#include "../Pathfinder/pathfinding.h"


//TODO: implement register Callback functionality
//TODO: implement iterative solving for multiSolver

namespace RunInterface {
    enum class RunnerReturnStatus{
        RETURN_NORMAL,
        RETURN_UNSOLVABLE,
        RETURN_LAST_SOLVER_DONE,
        RETURN_LAST_GRID_DONE,
    };


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

        void stepFinished(){};

        void gridFinished(){};

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
    Q_OBJECT
    public:
        explicit MultiRun(const RunGridConfig &thisConfig,
                          const std::list<Pathfinder::PathfinderStrategy> &thisStrats,
                          bool shouldRepeatUnsolvables);

        void nextStep() override;


    signals:

        void solverFinished(std::optional<Pathfinder::PathfinderPerformanceMetric>,
                            RunInterface::RunnerReturnStatus){};

    public slots:

        void createNewGridWithCurrentConfig();

        void onNewData(const RunInterface::RunGridConfig &thisConfig,
                       const std::list<Pathfinder::PathfinderStrategy> &thisStrats);

        void nextRun();

    private:
        void handleFinishedSolver();
        bool repeatUnsolvables;
        std::list<Pathfinder::PathfinderStrategy> strats;
        std::list<std::unique_ptr<Pathfinder::pathfindingParent>> solvers;
        std::list<std::unique_ptr<Pathfinder::pathfindingParent>>::iterator solverIterator;
        uint32_t gridIterator = 0;
        Pathfinder::pathfindingParent *currentSolver;
    };
}
