/**
 * @file grid.h
 * @brief Grid object definitions used in GridGenerator.
 */

#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cmath>

#include "cell.h"

// Predefined standard values for obstacle density and start-end distance
#define STANDARD_OBSTACLE_DENSITY 0.3
#define STANDARD_START_END_DISTANCE 0.5

/**
 * @namespace GridGenerator
 * @brief Namespace for classes and functionalities related to grid generator.
 */
namespace GridGenerator {
    /**
     * @struct GridCoordinate
     * @brief Struct which describes a coordinate point within the grid.
     */
    struct GridCoordinate {
        uint32_t x, y; ///< The x and y value of the coordinate

        /**
         * @brief Compare operator. Checks if the current GridCoordinate object is equal to another GridCoordinate object.
         * @param other GridCoordinate object to compare with.
         * @return Boolean value indicating whether the current object is equal to the other object.
         */
        [[nodiscard]] inline bool operator==(const GridCoordinate &other) const {
            return x == other.x && y == other.y;
        }

        /**
         * @brief Addition operator. Adds the x and y values of the current GridCoordinate object to another GridCoordinate object.
         * @param other GridCoordinate object to add with.
         * @return New GridCoordinate object after addition.
         */
        [[nodiscard]] inline GridCoordinate operator+(const GridCoordinate &other) const {
            return GridCoordinate(x + other.x, y + other.y);
        }

        /**
         * @fn getAbsDistanceTo
         * @brief Function to calculate absolute Distance between two points.
         * @param point The GridCoordinate value to which the distance is to be calculated.
         * @return The calculated distance.
         */
        [[nodiscard]] inline double getAbsDistanceTo(const GridCoordinate &point) const {
            return sqrt(pow(static_cast<double>(x) - point.x, 2) + pow(static_cast<double>(y) - point.y, 2));
        }

        /**
         * @fn getOctileDistanceTo
         * @brief Function to calculate octile Distance between two points (allows diagonal steps).
         * @param point The GridCoordinate value to which the distance is to be calculated.
         * @return The calculated distance.
         */
        [[nodiscard]] inline double getOctileDistanceTo(const GridCoordinate &point) const {
            uint32_t xDistAbs = std::abs(static_cast<int64_t>(x) - static_cast<int64_t>(point.x));
            uint32_t yDistAbs = std::abs(static_cast<int64_t>(y) - static_cast<int64_t>(point.y));
            if (xDistAbs > yDistAbs)
                return sqrt(2) * yDistAbs + 1 * (xDistAbs - yDistAbs);
            else
                return sqrt(2) * xDistAbs + 1 * (yDistAbs - xDistAbs);
        }

        /**
         * @fn getHash
         * @brief Function to compute a hash code for a GridCoordinate object.
         * @param c GridCoordinate object to compute hash code for.
         * @return Hash code as a size_t object.
         */
        static std::size_t getHash(const GridCoordinate &c) {
            return std::hash<uint32_t>()(c.x) ^ (std::hash<uint32_t>()(c.y) << 1);
        }

        /**
         * @fn getDirection
         * @brief Function to obtain the direction between two GridCoordinate objects as a pair of integers.
         * @param first First GridCoordinate object.
         * @param second Second GridCoordinate object.
         * @return A pair of integers, where each integer represents the direction in x-axis and y-axis respectively.
         */
        static std::pair<int8_t, int8_t> getDirection(const GridCoordinate &first, const GridCoordinate &second) {
            int64_t x = first.x - second.x;
            int64_t y = first.y - second.y;
            return std::make_pair(x == 0 ? 0 : x > 0 ? 1 : -1, y == 0 ? 0 : y > 0 ? 1 : -1);
        }
    };

    //forward declaration to avoid compile time errors
    class ObstacleGenerator;

    /**
     * @enum GridSolvedStatus
     * @brief Enum which describes the current status of the grid.
     */
    enum GridSolvedStatus {
        GRID_SOLVED,  ///< Indicates the grid has been successfully solved
        GRID_UNSOLVABLE, ///< Indicates the grid cannot be solved
        GRID_UNSOLVED ///< Indicates the grid has not yet been solved
    };

    /**
     * @class Grid
     * @brief Class which describes a 2D grid for a pathfinding problem.
     */
    class Grid {
    public:
        /**
         * @brief Constructs a Grid object.
         *
         * @param sizeX Horizontal (x-axis) size of the grid.
         * @param sizeY Vertical (y-axis) size of the grid.
         * @param generator Reference to an ObstacleGenerator object to generate obstacles for the pathfinding problem.
         * @param obstacleDensity The density of obstacles (0-1) present in the grid.
         * @param minStartEndDistance The minimum distance between the start and end cells in the pathfinding problem.
         */
        Grid(uint32_t sizeX, uint32_t sizeY, ObstacleGenerator &generator,
             float obstacleDensity = STANDARD_OBSTACLE_DENSITY,
             float minStartEndDistance = STANDARD_START_END_DISTANCE);

        /**
         * @brief Overloaded function call operator to access a cell at a specific grid coordinate.
         * May produce an std::out_of_range exception if coords are out of range.
         *
         * @param coords The grid coordinate at which the cell is to be accessed.
         * @return A reference to the cell at the specified coordinates.
         */
        Cell &operator()(GridCoordinate coords) {
            if (!isInBounds(coords.x, coords.y))
                throw std::out_of_range("Grid indices out of range");
            return cells[coords.x][coords.y];
        }

        /**
         * @brief Overloaded const function call operator to access a cell at a specific grid coordinate.
         * May produce an std::out_of_range exception if coords are out of range.
         *
         * @param coords The grid coordinate at which the cell is to be accessed.
         * @return A const reference to the cell at the specified coordinates.
         */
        const Cell &operator()(GridCoordinate coords) const {
            if (!isInBounds(coords.x, coords.y))
                throw std::out_of_range("Grid indices out of range");
            return cells[coords.x][coords.y];
        }

        /**
         * @fn isInBounds
         * @brief Checks if a given point is within the grid boundary.
         *
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @return True if the point lies within the grid boundary, False otherwise.
         */
        [[nodiscard]] inline bool isInBounds(int64_t x, int64_t y) const {
            return x >= 0 && x < sizeX && y >= 0 && y < sizeY;
        }

        /**
         * @fn markPathByParentCells
         * @brief Marks the cells in the found/is direct path by their parent cells.
         *
         * @param markByCellPointer A boolean value affecting the color representation of the cell.
         * If true, the marked cells will be represented based on the cell pointer. If false, they will be represented based on their positions.
         */
        void markPathByParentCells(bool markByCellPointer);

        /**
         * @fn getNeighborsCoordinates
         * @brief Fetches the neighboring coordinates of a given cell.
         *
         * @param coords The coordinates of the cell whose neighbors are to be found.
         * @return A vector of GridCoordinate objects representing the neighboring cells' coordinates, if any.
         */
        [[nodiscard]] std::vector<GridCoordinate> getNeighborsCoordinates(const GridCoordinate &coords) const;


        /**
         * @fn getStartCell
         * @brief Retrieve the start cell.
         *
         * @return Pointer to the start cell of the path.
         */
        [[nodiscard]] inline Cell *getStartCell() const { return startCell; }

        /**
         * @fn getStartCoordinates
         * @brief Retrieve the coordinates of the start cell.
         *
         * @return GridCoordinate of the start cell.
         */
        [[nodiscard]] inline GridCoordinate getStartCoordinates() const { return startCoordinates; }

        /**
         * @fn setStart
         * @brief Sets the starting coordinates of the Grid and the corresponding cell.
         *
         * @param startCoord Coordinate to set to the start position.
         */
        inline void setStart(const GridCoordinate &startCoord) {
            startCoordinates = startCoord;
            startCell = &(*this)(startCoord);
        }

        /**
         * @fn getEndCell
         * @brief Retrieve the end cell.
         *
         * @return Pointer to the end cell of the path.
         */
        [[nodiscard]] inline Cell *getEndCell() const { return endCell; }

        /**
         * @fn getEndCoordinates
         * @brief Retrieve the coordinates of the end cell.
         *
         * @return GridCoordinate of the end cell.
         */
        [[nodiscard]] inline GridCoordinate getEndCoordinates() const { return endCoordinates; }

        /**
         * @fn setEnd
         * @brief Sets the end coordinates of the Grid and the corresponding cell.
         *
         * @param endCoord Coordinate to set to the end position.
         */
        inline void setEnd(const GridCoordinate &endCoord) {
            endCoordinates = endCoord;
            endCell = &(*this)(endCoord);
        }

        /**
         * @fn getSizeX
         * @brief Retrieve the size of the grid along the x-axis.
         *
         * @return Size along the x-axis.
         */
        [[nodiscard]] inline uint32_t getSizeX() const { return cells.size(); }

        /**
         * @fn getSizeY
         * @brief Retrieve the size of the grid along the y-axis.
         *
         * @return Size along the y-axis.
         */
        [[nodiscard]] inline uint32_t getSizeY() const { return cells[0].size(); }

        /**
         * @fn incrementClosedCellCount
         * @brief Increases the count of closed cells by one.
         */
        inline void incrementClosedCellCount() { closedCellCount++; }

        /**
         * @fn incrementVisitedCellCount
         * @brief Increases the count of visited cells by one.
         */
        inline void incrementVisitedCellCount() { visitedCellCount++; }

        /**
         * @fn compareCells
         * @brief Comparator for the total cost of two cells (used in priority queues).
         *
         * @return The comparison function for the given cells.
         */
        auto compareCells() {
            return [this](const GridCoordinate &a, const GridCoordinate &b) {
                return (*this)(a).getCost().totalCost() > (*this)(b).getCost().totalCost();
            };
        }

        /**
         * @fn setSolved
         * @brief Marks the current Grid status as solved.
         */
        inline void setSolved() { exitStatus = GridSolvedStatus::GRID_SOLVED; }

        /**
         * @fn setUnsolvable
         * @brief Marks the current Grid status as unsolvable.
         */
        inline void setUnsolvable() { exitStatus = GridSolvedStatus::GRID_UNSOLVABLE; }

        /**
         * @fn getStatus
         * @brief Retrieves the current status of the Grid.
         *
         * @return The current Grid status.
         */
        [[nodiscard]] inline GridSolvedStatus getStatus() { return exitStatus; }

        /**
         * @fn getPathCellCount
         * @brief Retrieves the count of cells in the path.
         *
         * @return The number of cells in the path.
         */
        [[nodiscard]] inline uint32_t getPathCellCount() const { return pathCellCount; }

        /**
         * @fn getVisitedCellCount
         * @brief Retrieves the count of visited cells.
         *
         * @return The number of visited cells.
         */
        [[nodiscard]] inline uint32_t getVisitedCellCount() const { return visitedCellCount; }

        /**
         * @fn getClosedCellCount
         * @brief Retrieves the count of closed cells.
         *
         * @return The number of closed cells.
         */
        [[nodiscard]] inline uint32_t getClosedCellCount() const { return closedCellCount; }

        /**
         * @fn resetGrid
         * @brief Resets the current Grid object to its constructor state.
         */
        void resetGrid();

        /**< Static const field holding the possible directional offsets for calculating neighboring cells. */
        static const std::vector<std::pair<int8_t, int8_t>> offsets;

    private:
        /**< The horizontal size of the grid. */
        uint32_t sizeX;
        /**< The vertical size of the grid. */
        uint32_t sizeY;

        /**< 2D vector containing cells of the grid. */
        std::vector<std::vector<Cell>> cells;

        /**< Pointer to the start cell. */
        Cell *startCell;
        /**< Pointer to the end cell. */
        Cell *endCell;

        /**< Coordinates of the start cell. */
        GridCoordinate startCoordinates;
        /**< Coordinates of the end cell. */
        GridCoordinate endCoordinates;

        /**< The current solved state of the pathfinding problem. */
        GridSolvedStatus exitStatus;

        /**< Number of cells in the current path. */
        uint32_t pathCellCount = 1;
        /**< Number of closed cells in the search algorithm. */
        uint32_t closedCellCount = 0;
        /**< Number of visited cells in the search algorithm. */
        uint32_t visitedCellCount = 0;
    };


}