#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cmath>

#include "cell.h"

//TODO: documentation

#define STANDARD_OBSTACLE_DENSITY 0.3
#define STANDARD_START_END_DISTANCE 0.5


namespace GridGenerator {
    struct GridCoordinate {
        uint32_t x;
        uint32_t y;

        [[nodiscard]] inline double getAbsDistanceTo(const GridCoordinate &point) const {
            return sqrt(pow(static_cast<double>(x) - point.x, 2) + pow(static_cast<double>(y) - point.y, 2));
        }

        [[nodiscard]] inline double getOctileDistanceTo(const GridCoordinate &point) const {
            uint32_t xDistAbs = std::abs(static_cast<int64_t>(x) - static_cast<int64_t>(point.x));
            uint32_t yDistAbs = std::abs(static_cast<int64_t>(y) - static_cast<int64_t>(point.y));
            if (xDistAbs > yDistAbs)
                return sqrt(2) * yDistAbs + 1 * (xDistAbs - yDistAbs);
            else
                return sqrt(2) * xDistAbs + 1 * (yDistAbs - xDistAbs);
        }


        [[nodiscard]] inline bool operator==(const GridCoordinate &other) const {
            return x == other.x && y == other.y;
        }

        [[nodiscard]] inline GridCoordinate operator+(const GridCoordinate &other) const {
            return GridCoordinate(x + other.x, y + other.y);
        }

        static std::size_t getHash(const GridCoordinate &c) {
            return std::hash<uint32_t>()(c.x) ^ (std::hash<uint32_t>()(c.y) << 1);
        }

        static std::pair<int8_t, int8_t> getDirection(const GridCoordinate &first, const GridCoordinate &second) {
            int64_t x = first.x - second.x;
            int64_t y = first.y - second.y;
            return std::make_pair(x == 0 ? 0 : x > 0 ? 1 : -1, y == 0 ? 0 : y > 0 ? 1 : -1);
        }
    };

    //forward declaration to avoid compile time errors
    class ObstacleGenerator;

    enum GridSolvedStatus {
        GRID_SOLVED,
        GRID_UNSOLVABLE,
        GRID_UNSOLVED
    };


    class Grid {
    public:
        Grid(uint32_t sizeX, uint32_t sizeY, ObstacleGenerator &generator,
             float obstacleDensity = STANDARD_OBSTACLE_DENSITY,
             float minStartEndDistance = STANDARD_START_END_DISTANCE);

        Cell &operator()(GridCoordinate coords) {
            if (coords.x >= sizeX || coords.y >= sizeY)
                throw std::out_of_range("Grid indices out of range");
            return cells[coords.x][coords.y];
        }

        const Cell &operator()(GridCoordinate coords) const {
            if (coords.x >= sizeX || coords.y >= sizeY)
                throw std::out_of_range("Grid indices out of range");
            return cells[coords.x][coords.y];
        }

        void markPathByParentCells(bool markByCellPointer);


        std::vector<std::reference_wrapper<Cell>> getNeighborsCells(GridCoordinate coords);

        [[nodiscard]] std::vector<GridCoordinate> getNeighborsCoordinates(const GridCoordinate &coords) const;

        [[nodiscard]] inline bool isInBounds(int64_t x, int64_t y) const {
            return x >= 0 && x < sizeX && y >= 0 && y < sizeY;
        }

        [[nodiscard]] inline Cell *getStartCell() const { return startCell; }

        [[nodiscard]] inline Cell *getEndCell() const { return endCell; }

        [[nodiscard]] inline uint32_t getSizeX() const { return cells.size(); }

        [[nodiscard]] inline uint32_t getSizeY() const { return cells[0].size(); }

        [[nodiscard]] inline GridCoordinate getStartCoordinates() const { return startCoordinates; }

        [[nodiscard]] inline GridCoordinate getEndCoordinates() const { return endCoordinates; }

        inline void incrementClosedCellCount() { closedCellCount++; }

        inline void incrementVisitedCellCount() { visitCount++; }

        inline void setStart(const GridCoordinate &startCoord) {
            startCoordinates = startCoord;
            startCell = &(*this)(startCoord);
        }

        inline void setEnd(const GridCoordinate &endCoord) {
            endCoordinates = endCoord;
            endCell = &(*this)(endCoord);
        }

        auto compareCells() {
            return [this](const GridCoordinate &a, const GridCoordinate &b) {
                return (*this)(a).getCost().totalCost() > (*this)(b).getCost().totalCost();
            };
        }

        inline void setSolved() { exitStatus = GridSolvedStatus::GRID_SOLVED; }

        inline void setUnsolvable() { exitStatus = GridSolvedStatus::GRID_UNSOLVABLE; }

        void resetGrid();

        static const std::vector<std::pair<int8_t, int8_t>> offsets;
    private:

        uint32_t sizeX;
        uint32_t sizeY;
        std::vector<std::vector<Cell>> cells;

        Cell *startCell;
        Cell *endCell;

        GridCoordinate startCoordinates;
        GridCoordinate endCoordinates;

        GridSolvedStatus exitStatus;

        uint32_t pathCellCount = 1;
        uint32_t closedCellCount = 0;
        uint32_t visitCount = 0;
    };


}