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

#include "GridGenerator/grid.h"
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
        /**< Count of cells that are part of the path */
        uint32_t pathCells;
        /**< Count of cells that have been visited */
        uint32_t visitedCells;
        /**< Count of cells that have been closed */
        uint32_t closedCells;
        /**< Total time taken for solving in micro seconds */
        double solvingUSeconds;
        /**< Average time taken per step in micro seconds */
        double avgUSecondsPerStep;
        /**< Total number of steps taken in the algorithm */
        uint32_t stepCount;
        /**< Strategy used for the pathfinding */
        PathfinderStrategy strat;

    };

    // Forward declaration to avoid compile-time errors
    class pathfindingParent;

    /**
     * @struct PathfinderStrategyParser
     * @brief Struct to parse PathfinderStrategy type
     */
    struct PathfinderStrategyParser {
        /**< Maps PathfinderStrategy to its displayable string representation*/
        static const std::map<PathfinderStrategy, std::string> pathfindingStrategyToDisplayableText;

        /**
         * @fn parsePathfinderStrategy
         * @brief Static function that parses a PathfinderStrategy enum and returns a unique_ptr of the corresponding pathfinding parent object.
         * @param strat The pathfinding strategy to use.
         * @param grid The grid on which to apply the strategy.
         * @return A unique_ptr to the generated pathfinding object.
         */
        static std::unique_ptr<pathfindingParent> parsePathfinderStrategy(PathfinderStrategy strat,
                                                                          GridGenerator::Grid &grid);
    };

    /**
     * @class pathfindingParent
     * @brief The parent pathfinding class. Contains the grid and the main functions of pathfinding.
     */
    class pathfindingParent {
    public:
        /**
         * @brief Constructor for the pathfindingParent class.
         * @param grid Reference to the Grid object used in the pathfinding.
         */
        explicit pathfindingParent(GridGenerator::Grid &grid) :
                grid(grid) {
            initGenericSolver();
        }

        /**
         * @brief Destructor for the pathfindingParent class.
         */
        virtual ~pathfindingParent() = default;

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
        inline PathfinderPerformanceMetric getPerformanceMetric() {
            setStrat();
            return PathfinderPerformanceMetric{grid.getPathCellCount(),
                                               grid.getVisitedCellCount(),
                                               grid.getClosedCellCount(),
                                               timer.getAbsoluteTime(),
                                               timer.getAvgUSecondsPerStep(),
                                               timer.getStepCount(),
                                               strat};
        }

    protected:
        /**
         * @fn virtual setStrat
         * @brief Abstract method for setting the pathfinding strategy.
         */
        virtual void setStrat() = 0;

        /**
         * @fn isCellBlockedOrOutOfBounds
         * @brief Checks if a cell is blocked or out of bounds.
         * @param x:x-coordinate of the cell.
         * @param y:y-coordinate of the cell.
         * @return Returns true if the cell is either blocked or out of bounds, false otherwise
         */
        bool isCellBlockedOrOutOfBounds(int64_t x, int64_t y);

        /**< Strategy of the implemented Pathfinder */
        PathfinderStrategy strat;

        /**< Timer object for evaluation pathfinder performance */
        PathfinderTimer timer;

        /**< Reference to the grid which the pathfinder is working on */
        GridGenerator::Grid &grid;

        /**< Priority queue of the next Cells to be evluated */
        std::priority_queue<GridGenerator::GridCoordinate, std::vector<GridGenerator::GridCoordinate>,
                decltype(grid.compareCells())> nextCellQueue{grid.compareCells()};

    private:
        /**
         * @fn void initGenericSolver();
         * @brief Initializes the general solver.
         */
        void initGenericSolver();
    };

    /**
     * @class DijkstraSolve
     * @brief Extension of the pathfindingParent class. Defines an object which applies Dijkstra's algorithm for pathfinding.
     */
    class DijkstraSolve : public pathfindingParent {
    public:
        /**
         * @brief Constructor for the DijkstraSolve class. Uses the parent Class constructor.
         * @param grid Reference to the Grid object where the Dijkstra algorithm will be applied.
         */
        explicit DijkstraSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        /**
         * @fn void nextStep
         * @brief Overrides the nextStep() function to implement the Dijkstra algorithm's next step.
         */
        void nextStep() override;

    private:
        /**
         * @fn setStrat
         * @brief Overrides the setStrat() function to set the strategy to Dijkstra.
         */
        inline void setStrat() override { strat = PathfinderStrategy::PATHFINDER_DIJKSTRA; }
    };

    /**
     * @class AStarSolve
     * @brief Extension of the pathfindingParent class. Defines an object which applies the A* algorithm for pathfinding.
     */
    class AStarSolve : public pathfindingParent {
    public:
        /**
         * @brief Constructor for the AStarSolve class. Uses the parent Class constructor.
         * @param grid Reference to the Grid object where the A* algorithm will be applied.
         */
        explicit AStarSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        /**
         * @fn void nextStep
         * @brief Overrides the nextStep() function to implement the Dijkstra algorithm's next step.
         */
        void nextStep() override;

    private:
        /**
         * @fn setStrat
         * @brief Overrides the setStrat() function to set the strategy to Dijkstra.
         */
        inline void setStrat() override { strat = PathfinderStrategy::PATHFINDER_A_STAR; }
    };

    /**
     * @class JumpPointSolve
     * @brief Extension of the pathfindingParent class. Defines an object which applies the Jump Point Search algorithm for pathfinding.
     */
    class JumpPointSolve : public pathfindingParent {
    public:
        /**
         * @brief Constructor for the JumpPointSolve class. Uses the parent Class constructor.
         * @param grid Reference to the Grid object where the Jump-Point-Search algorithm will be applied.
         */
        explicit JumpPointSolve(GridGenerator::Grid &grid) : pathfindingParent(grid) {}

        /**
         * @fn void nextStep
         * @brief Overrides the nextStep() function to implement the Dijkstra algorithm's next step.
         */
        void nextStep() override;

    private:
        /**
         * @fn setStrat
         * @brief Overrides the setStrat() function to set the strategy to Dijkstra.
         */
        inline void setStrat() override { strat = PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH; }

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