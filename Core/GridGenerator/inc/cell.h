/**
 * @file cell.h
 * @brief Cell object definitions used in GridGenerator.
 */

#pragma once

#include <QColor>
#include <variant>

/**
 * @namespace GridGenerator
 * @brief Namespace for classes and functionalities related to grid generator.
 */
namespace GridGenerator {

    /**
     * @enum CellState
     * @brief An enumeration of possible states of a cell on the grid.
     *
     * These states dictate how the cell can be interacted with, such as whether it can be traversed or not.
     */
    enum class CellState {
        CELL_OBSTACLE = 0, /**< Represents cells that are obstacles. */
        CELL_OPEN = 1, /**< Represents open cells. */
        CELL_VISITED = 2, /**< Represents cells that have been visited. */
        CELL_CLOSED = 3, /**< Represents cells that have been closed. */
        CELL_PATH = 4 /**< Represents cells that are part of the path. */
    };

    /**
     * @struct CellStateParser
     * @brief Structure to parse cell states.
     */
    struct CellStateParser {
        /**
         * @fn parseCellStateToQColor
         * @brief Parses cell state to QColor.
         * @param state The state of the cell.
         * @param startEnd A boolean indicating if the cell is a starting or end cell.
         * @return QColor corresponding to the cell state.
         */
        static QColor parseCellStateToQColor(CellState state, bool &&startEnd = false);
    };

    /**
     * @struct CellCost
     * @brief Structure to hold the costs associated with a cell in the grid.
     */
    struct CellCost {
        double gCost; /**< Represents the g cost of the cell in the pathfinding algorithm. */
        double hCost; /**< Represents the heuristic cost of the cell in the pathfinding algorithm. */

        /**
         * @fn totalCost
         * @brief Computes the total cost for a cell.
         * @return The sum of the g and h costs.
         */
        [[nodiscard]] inline double totalCost() const { return gCost + hCost; }
    };


    /**
     * @class Cell
     * @brief Class representing a cell in the grid.
     */
    class Cell {
        /// @brief Type for parent of a cell. Can be a pointer to the parent Cell or a pair representing the direction to the parent.
        using CellParentType = std::variant<Cell *, std::pair<uint32_t, uint32_t>>;

    public:
        /**
         * @brief Default constructor for Cell class. Initializes state to CELL_OPEN, cost to an empty CellCost object, and parent to nullptr.
         */
        Cell() : state(CellState::CELL_OPEN), cost(CellCost{}), parent(nullptr) {}

        /**
         * @fn markObstacle
         * @brief Marks the cell as an obstacle.
         */
        inline void markObstacle() { state = CellState::CELL_OBSTACLE; }

        /**
         * @fn markOpen
         * @brief Marks the cell as open.
         */
        inline void markOpen() { state = CellState::CELL_OPEN; }

        /**
         * @fn markVisited
         * @brief Marks the cell as visited.
         */
        inline void markVisited() { state = CellState::CELL_VISITED; }

        /**
         * @fn markClosed
         * @brief Marks the cell as closed.
         */
        inline void markClosed() { state = CellState::CELL_CLOSED; }

        /**
         * @fn markPath
         * @brief Marks the cell as part of the final path.
         */
        inline void markPath() { state = CellState::CELL_PATH; }

        /**
        * @fn setGCost
        * @brief Sets the actual cost from the start node to this cell (gCost).
        * @param gCost The gCost value to be set.
        */
        inline void setGCost(double gCost) { cost.gCost = gCost; }

        /**
        * @fn setHCost
        * @brief Sets heuristic estimated cost from this cell to the end node (hCost).
        * @param hCost The hCost value to be set.
        */
        inline void setHCost(double hCost) { cost.hCost = hCost; }

        /**
        * @fn setParentCellPointer
        * @brief Sets the parent of this cell.
        * @param parentCell The parent cell to be set.
        */
        inline void setParentCellPointer(Cell *parentCell) { parent = parentCell; }

        /**
        * @fn setParentCoordinates
        * @brief Sets the coordinates of the parents of this cell.
        * @param parentCoords The parent coordinates to be set.
        */
        inline void setParentCoordinates(std::pair<uint32_t, uint32_t> parentCoords) {
            parent = std::move(parentCoords);
        }

        /**
        * @fn getParentIfCellPointer
        * @brief Gets the parent of the cell if it's a Cell pointer.
        * @return Pointer to the parent Cell if cell's parent is a Cell pointer, nullptr otherwise.
        */
        [[nodiscard]] inline Cell **getParentIfCellPointer() { return std::get_if<Cell *>(&parent); }

        /**
        * @fn getParentIfDirPair
        * @brief Gets the direction to the parent of the cell if it's a direction pair.
        * @return Pointer to the parent direction pair if cell's parent is a pair, nullptr otherwise.
        */
        [[nodiscard]] inline std::pair<uint32_t, uint32_t> *getParentIfCoord() {
            return std::get_if<std::pair<uint32_t, uint32_t>>(&parent);
        }

        /**
        * @fn getState
        * @brief Access the state of the cell.
        * @return The current state of the cell.
        */
        [[nodiscard]] inline CellState getState() const { return state; }

        /**
        * @fn getCost
        * @brief Access the cost of the cell.
        * @return The current cost associated with the cell.
        */
        [[nodiscard]] inline CellCost getCost() const { return cost; }

    private:
        CellState state; /**< Indicates the current state of the cell. */
        CellCost cost; /**< Contains the cost information related to the cell. */
        CellParentType parent; /**< Parent cell or the direction pair to get to the parent cell. */
    };
}