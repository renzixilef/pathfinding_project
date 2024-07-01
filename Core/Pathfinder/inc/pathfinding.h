/**
 * @file pathfinding.h
 * @brief This file contains classes and enums that define/implement pathfinding strategies on a given grid.
 */

#pragma once

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <queue>
#include <optional>
#include "grid.h"
#include "timer.h"


/**
 * @namespace Pathfinder
 * @brief Namespace for classes and functionalities related to pathfinding algorithms.
 */
namespace Pathfinder {

    /**
     * @enum PathfinderStrategy
     * @brief The strategy used for pathfinding.
     */
    enum class PathfinderStrategy {
        PATHFINDER_DIJKSTRA = 1, /**< Dijkstra's algorithm pathfinding. */
        PATHFINDER_A_STAR = 2, /**< A* algorithm pathfinding. */
        PATHFINDER_JUMP_POINT_SEARCH = 3 /**< Jump-Point-Search algorithm pathfinding. */
    };

    /**
     * @struct PathfinderPerformanceMetric
     * @brief Struct to hold the performance metrics of a pathfinding strategy.
     */
    struct PathfinderPerformanceMetric {
        uint32_t pathCells;/**< Count of cells that are part of the path */
        uint32_t visitedCells; /**< Count of cells that have been visited */
        uint32_t closedCells; /**< Count of cells that have been closed */
        double solvingUSeconds; /**< Total time taken for solving in micro seconds */
        double avgUSecondsPerStep; /**< Average time taken per step in micro seconds */
        uint32_t stepCount; /**< Total number of steps taken in the algorithm */
        PathfinderStrategy strat; /**< Strategy used for the pathfinding */

    };

    // Forward declaration to avoid compile-time errors
    class PathfindingParent;

    /**
     * @struct PathfinderStrategyParser
     * @brief Struct to parse PathfinderStrategy type
     */
    struct PathfinderStrategyParser {
        /// @brief Maps PathfinderStrategy to its displayable string representation
        static const std::map<PathfinderStrategy, std::string> pathfindingStrategyToDisplayableText;

        /**
         * @fn parsePathfinderStrategy
         * @brief Static function that parses a PathfinderStrategy enum and returns a unique_ptr of the corresponding pathfinding parent object.
         * @param strat The pathfinding strategy to use.
         * @param grid The grid on which to apply the strategy.
         * @return A unique_ptr to the generated pathfinding object.
         */
        static std::unique_ptr<PathfindingParent> parsePathfinderStrategy(PathfinderStrategy strat,
                                                                          GridGenerator::Grid &grid);
    };

    /**
     * @class PathfindingParent
     * @brief The parent pathfinding class. Contains the grid and the main functions of pathfinding.
     */
    class PathfindingParent {
    public:
        /**
         * @brief Constructor for the PathfindingParent class.
         * @param grid Reference to the Grid object used in the pathfinding.
         */
        explicit PathfindingParent(GridGenerator::Grid &grid) :
                grid(grid),
                nextCellQueue(grid.compareCells()) {
            initGenericSolver();
        }

        /**
         * @brief Destructor for the PathfindingParent class.
         */
        virtual ~PathfindingParent() = default;

        /**
         * @fn queueEmpty
         * @brief Checks if the next cell queue is empty.
         * @return Returns true if queue is empty, false otherwise.
         */
        inline bool queueEmpty() { return nextCellQueue.empty(); }

        /**
         * @fn virtual nextStep
         * @brief Abstract method to implement the next step of the pathfinding algorithm.
         */
        virtual void nextStep() = 0;

        /**
         * @fn getPerformanceMetric
         * @brief Get a structure containing performance metrics of the algorithm.
         * @return Returns a PathfinderPerformanceMetric structure.
         */
        [[nodiscard]] inline PathfinderPerformanceMetric getPerformanceMetric() const {
            return PathfinderPerformanceMetric{grid.getPathCellCount(),
                                               grid.getVisitedCellCount(),
                                               grid.getClosedCellCount(),
                                               timer.getAbsoluteTime(),
                                               timer.getAvgUSecondsPerStep(),
                                               timer.getStepCount(),
                                               getStrat()};
        }

        /**
         * @fn solveNoWait
         * @brief Solves the grid without using the regular runnerInterface, just for debugging and testing purposes
         */
        [[maybe_unused]] void solveNoWait();

        /**
         * @fn reInit
         * @brief re initializes the queue and the start end node cost calculation
         */
        void reInit();

    protected:
        using compareCellsFunction = std::function<bool(const GridGenerator::GridCoordinate &,
                                                        const GridGenerator::GridCoordinate &)>;

        /**
         * @fn virtual getStrat
         * @brief Abstract method for getting the pathfinding strategy.
         */
        [[nodiscard]] virtual PathfinderStrategy getStrat() const = 0;

        /**
         * @fn isCellBlockedOrOutOfBounds
         * @brief Checks if a cell is blocked or out of bounds.
         * @param x:x-coordinate of the cell.
         * @param y:y-coordinate of the cell.
         * @return Returns true if the cell is either blocked or out of bounds, false otherwise
         */
        bool isCellBlockedOrOutOfBounds(int64_t x, int64_t y);

        PathfinderTimer timer; /**< Timer object for evaluation pathfinder performance */

        GridGenerator::Grid &grid; /**< Reference to the grid which the pathfinder is working on */

        /// @brief Priority queue of the next Cells to be evluated
        std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
                compareCellsFunction > nextCellQueue;

    private:
        /**
         * @fn void initGenericSolver();
         * @brief Initializes the general solver.
         */
        void initGenericSolver();
    };

    /**
     * @class DijkstraSolve
     * @brief Extension of the PathfindingParent class. Defines an object which applies Dijkstra's algorithm for pathfinding.
     */
    class DijkstraSolve : public PathfindingParent {
    public:
        /**
         * @brief Constructor for the DijkstraSolve class. Uses the parent Class constructor.
         * @param grid Reference to the Grid object where the Dijkstra algorithm will be applied.
         */
        explicit DijkstraSolve(GridGenerator::Grid &grid) : PathfindingParent(grid) {}

        /**
         * @fn void nextStep
         * @brief Overrides the nextStep() function to implement the Dijkstra algorithm's next step.
         */
        void nextStep() override;

    private:
        /**
         * @fn getStrat
         * @brief Overrides the getStrat() function to get the strategy (Dijkstra).
         */
        [[nodiscard]] inline PathfinderStrategy getStrat() const override {
            return PathfinderStrategy::PATHFINDER_DIJKSTRA;
        }
    };

    /**
     * @class AStarSolve
     * @brief Extension of the PathfindingParent class. Defines an object which applies the A* algorithm for pathfinding.
     */
    class AStarSolve : public PathfindingParent {
    public:
        /**
         * @brief Constructor for the AStarSolve class. Uses the parent Class constructor.
         * @param grid Reference to the Grid object where the A* algorithm will be applied.
         */
        explicit AStarSolve(GridGenerator::Grid &grid) : PathfindingParent(grid) {}

        /**
         * @fn void nextStep
         * @brief Overrides the nextStep() function to implement the Dijkstra algorithm's next step.
         */
        void nextStep() override;

    private:
        /**
         * @fn getStrat
         * @brief Overrides the getStrat() function to get the strategy (A*).
         */
        [[nodiscard]] inline PathfinderStrategy getStrat() const override {
            return PathfinderStrategy::PATHFINDER_A_STAR;
        }
    };

    /**
     * @class JumpPointSolve
     * @brief Extension of the PathfindingParent class. Defines an object which applies the Jump Point Search algorithm for pathfinding.
     */
    class JumpPointSolve : public PathfindingParent {
    public:
        /**
         * @brief Constructor for the JumpPointSolve class. Uses the parent Class constructor.
         * @param grid Reference to the Grid object where the Jump-Point-Search algorithm will be applied.
         */
        explicit JumpPointSolve(GridGenerator::Grid &grid) : PathfindingParent(grid) {}

        /**
         * @fn void nextStep
         * @brief Overrides the nextStep() function to implement the Dijkstra algorithm's next step.
         */
        void nextStep() override;

    private:
        /**
         * @fn getStrat
         * @brief Overrides the getStrat() function to get the strategy (JPS).
         */
        [[nodiscard]] inline PathfinderStrategy getStrat() const override {
            return PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH;
        }

        /**
         * @fn jump
         * @brief Performs a "jump" operation in the JPS algorithm, which attempts to prune unnecessary nodes.
         * @param currentCoord Current coordinate being analyzed.
         * @param direction Direction in which to "jump".
         * @return Returns an optional GridCoordinate, which has a value if a jump point is found.
         */
        std::optional<GridGenerator::GridCoordinate> jump(GridGenerator::GridCoordinate currentCoord,
                                                          std::pair<int8_t, int8_t> direction);
    };

}