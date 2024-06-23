/**
 * @file runner.h
 * @brief This file contains Runner interface classes that manage different possible interactions with the backend.
 */

#pragma once

#include <cstdint>
#include <list>
#include <QObject>

#include "../GridGenerator/obstacle_gen.h"
#include "../Pathfinder/pathfinding.h"

/**
 * @namespace RunInterface
 * @brief Namespace for classes and functionalities related to interfacing the front- and backend.
 */
namespace RunInterface {
    /**
     * @enum RunnerReturnStatus
     * @brief Enumeration defining the return status after running a pathfinding algorithm.
     */
    enum class RunnerReturnStatus {
        RETURN_NORMAL = 0, /**< Normal return status. */
        RETURN_UNSOLVABLE = 1, /**< Grid unsolvable return status. */
        RETURN_LAST_SOLVER_DONE = 2, /**< Last Solver of one grid iteration done return status. */
        RETURN_LAST_GRID_DONE = 3, /**< Last Grid of one config done return status. */
    };

    /**
     * @struct RunGridConfig
     * @brief Data structure to hold the grid configuration for running a pathfinding algorithm.
     */
    struct RunGridConfig {
        uint32_t gridWidth{}; /**< Grid width */
        uint32_t gridHeight{}; /**< Grid height */
        float obstacleDensity{}; /**< Density of obstacles in the grid */
        float minStartEndDistance{}; /**< Minimum distance between start and end points */
        /// @brief Obstacle generation strategy
        GridGenerator::ObstacleGenStrategy obstacleGenStrategy{GridGenerator::ObstacleGenStrategy::OBSTACLE_RANDOM};
        std::optional<uint32_t> iterations = std::nullopt; /**< Optional iterations count */

        bool operator<(const RunGridConfig& other) const {
            return std::tie(gridWidth, gridHeight, obstacleDensity, minStartEndDistance, obstacleGenStrategy) <
                   std::tie(other.gridWidth, other.gridHeight, other.obstacleDensity, other.minStartEndDistance,
                            other.obstacleGenStrategy);
        }
    };

    /**
     * @class RunnerParent
     * @brief Parent class for the interface.
     */
    class RunnerParent : public QObject {
    Q_OBJECT
    public:
        /**
         * @brief Constructor for RunnerParent taking a RunGridConfig argument.
         * @param thisConfig: the RunGridConfig structure defining the configuration for the runner.
         */
        explicit RunnerParent(const RunGridConfig &thisConfig);

        /**
         * @fn getGridRef
         * @brief Method to get a reference to the grid currently worked on.
         * @return Return a reference to the grid.
         */
        [[nodiscard]] const inline GridGenerator::Grid &getGridRef() const { return grid; }

    signals:
        /**< Signal emitted when a step is finished. *//**< Signal emitted when a step is finished. */
        void stepFinished();
        /**< Signal emitted when grid is finished. */
        void gridFinished();

    public slots:
        /**
         * @fn virtual nextStep
         * @brief Pure virtual method to be overridden by derived classes to start the next step of the pathfinder.
         */
        virtual void nextStep() = 0;

    protected:
        RunGridConfig config; /**< Configuration data of the runner. */
        GridGenerator::Grid grid; /**< The current Grid that the runner is working on. */
    };

    /**
     * @class SingleRun
     * @brief This class extends the RunnerParent class and is designed to run a single pathfinding algorithm.
     */
    class SingleRun : public RunnerParent {
    Q_OBJECT
    public:
        /**
         * @brief Explicit constructor for the SingleRun class.
         * @param thisConfig A reference to the RunGridConfig object that defines the configuration for the runner.
         * @param thisStrat A reference to the PathfinderStrategy object that defines the strategy the pathfinder should use.
         */
        explicit SingleRun(const RunGridConfig &thisConfig, const Pathfinder::PathfinderStrategy &thisStrat);

        /**
         * @fn nextStep
         * @brief Overridden method from RunnerParent class to start the next step of the pathfinder.
         */
        void nextStep() override;

    public slots:
        /**
         * @fn onRunReset
         * @brief Slot that defines the action to be taken on resetting the running of pathfinding algorithm.
         */
        void onRunReset();

    private:
        Pathfinder::PathfinderStrategy strat; /**< holds the pathfinder strategy to be used */
        /// @brief holds a unique pointer to the pathfinding object corresponding to strat
        std::unique_ptr<Pathfinder::PathfindingParent> solver;
    };

    /**
     * @class MultiRun
     * @brief This class extends the RunnerParent class and is designed to run multiple pathfinding algorithms and grid configs.
     */
    class MultiRun : public RunnerParent {
    Q_OBJECT
    public:
        /**
         * @brief Explicit constructor for the MultiRun class.
         * @param thisConfig A RunGridConfig reference defining the configuration for pathfinding runs.
         * @param thisStrats A list of PathfinderStrategy objects representing the multiple strategies to run.
         * @param shouldRepeatUnsolvables A boolean to indicate whether unsolvable grids should be regenerated.
         */
        explicit MultiRun(const RunGridConfig &thisConfig,
                          const std::list<Pathfinder::PathfinderStrategy> &thisStrats,
                          bool shouldRepeatUnsolvables);

        /**
         * @fn nextStep
         * @brief Overridden method from RunnerParent class to start the next step of the pathfinder.
         */
        void nextStep() override;

    signals:
        /**
         * @fn solverFinished
         * @brief Signal to indicate that a solver has finished.
         * @param PathfinderPerformanceMetric Object holding the performance metric of the solver.
         * @param int32_t Represents the exit Status of the runner as RunnerReturnStatus.
         */
        void solverFinished(const Pathfinder::PathfinderPerformanceMetric &,
                            int32_t);

    public slots:
        /**
         * @fn createNewGridWithCurrentConfig
         * @brief Slot to handle creating a new grid based on the current configuration.
         */
        void createNewGridWithCurrentConfig();

        /**
         * @fn onNewData
         * @brief Slot to handle new data transmission.
         * @param thisConfig The RunGridConfig object defining the new run configuration.
         * @param thisStrats List of the pathfinder strategies to utilise for the next runs.
         */
        void onNewData(const RunInterface::RunGridConfig &thisConfig,
                       const std::list<Pathfinder::PathfinderStrategy> &thisStrats);

        /**
         * @fn nextRun
         * @brief Slot to handle the action of moving to the next run.
         */
        void nextRun();

    private:
        /**
         * @fn handleFinishedSolver
         * @brief Helper function to handle the action after a solver has finished.
         */
        void handleFinishedSolver();

        bool repeatUnsolvables; /**< indicates whether unsolvable grids should be generated again */
        std::list<Pathfinder::PathfinderStrategy> strats; /**< List of current config solver strategies. */
        std::list<std::unique_ptr<Pathfinder::PathfindingParent>> solvers; /**< List of current solver instances. */
        std::list<std::unique_ptr<Pathfinder::PathfindingParent>>::iterator solverIterator; /**< Iterator for solver instances. */
        uint32_t gridIterator = 0; /**< Keeping track of grid count for multi-grid scenarios. */
        Pathfinder::PathfindingParent *currentSolver; /**< Current solver instance. */
    };
}
