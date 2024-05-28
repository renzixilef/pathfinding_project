#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cmath>

#include "cell.h"

//TODO: documentation

namespace GridGenerator {
    struct GridCoordinate {
        uint32_t x;
        uint32_t y;

        [[nodiscard]] inline float getAbsDistanceTo(const GridCoordinate &point) const {
            return static_cast<float>(sqrt(pow(x - point.x, 2) + pow(y - point.y, 2)));
        }

        [[nodiscard]] inline bool operator==(const GridCoordinate &other) const {
            return x == other.x && y == other.y;
        }
        static std::size_t getHash(const GridCoordinate& c) {
            return std::hash<uint32_t>()(c.x) ^ std::hash<uint32_t>()(c.y);
        }
    };

    //forward declaration to avoid compile time errors
    class ObstacleGenerator;

    enum GridSolvedStatus{
        GRID_SOLVED,
        GRID_UNSOLVABLE,
        GRID_UNSOLVED
    };


    class Grid {
    public:
        Grid(uint32_t sizeX, uint32_t sizeY, float obstacleDensity, ObstacleGenerator &generator);

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

        void markPathByParentCells();


        std::vector<std::reference_wrapper<Cell>> getNeighborsCells(GridCoordinate coords);

        [[nodiscard]] std::vector<GridCoordinate> getNeighborsCoordinates(const GridCoordinate &coords) const;


        [[nodiscard]] inline Cell *getStartCell() const { return startCell; }

        [[nodiscard]] inline Cell *getEndCell() const { return endCell; }

        [[nodiscard]] inline uint32_t getSizeX() const {return cells.size();}

        [[nodiscard]] inline uint32_t getSizeY() const {return cells[0].size();}

        [[nodiscard]] inline GridCoordinate getStartCoordinates() const { return startCoordinates; }

        [[nodiscard]] inline GridCoordinate getEndCoordinates() const { return endCoordinates; }

        auto compareCells() {
            return [this](const GridCoordinate &a, const GridCoordinate &b) {
                return (*this)(a).getCost().totalCost() < (*this)(b).getCost().totalCost();
            };
        }

        inline void setSolved(){exitStatus = GridSolvedStatus::GRID_SOLVED;}
        inline void setUnsolvable(){exitStatus = GridSolvedStatus::GRID_UNSOLVABLE;}

    private:

        uint32_t sizeX;
        uint32_t sizeY;
        std::vector<std::vector<Cell>> cells;

        Cell *startCell;
        Cell *endCell;

        GridCoordinate startCoordinates;
        GridCoordinate endCoordinates;

        GridSolvedStatus exitStatus;
    };


}