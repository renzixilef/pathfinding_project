#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

#include "cell.h"
#include "obstacle_gen.h"

//TODO: documentation

namespace GridGenerator {
    struct GridCoordinates {
        uint32_t x;
        uint32_t y;

        [[nodiscard]] inline float getAbsDistanceTo(const GridCoordinates &point) const {
            return static_cast<float>(sqrt(pow(x - point.x, 2) + pow(y - point.y, 2)));
        }

        [[nodiscard]] inline bool operator==(const GridCoordinates &other) const {
            return x == other.x && y == other.y;
        }
    };

    enum GridSolvedStatus{
        GRID_SOLVED,
        GRID_UNSOLVABLE,
        GRID_UNSOLVED
    };


    class Grid {
    public:
        Grid(uint32_t sizeX, uint32_t sizeY, float obstacleDensity, ObstacleGenerator &generator);

        Cell &operator()(GridCoordinates coords) {
            if (coords.x >= sizeX || coords.y >= sizeY)
                throw std::out_of_range("Grid indices out of range");
            return cells[coords.x][coords.y];
        }

        const Cell &operator()(GridCoordinates coords) const {
            if (coords.x >= sizeX || coords.y >= sizeY)
                throw std::out_of_range("Grid indices out of range");
            return cells[coords.x][coords.y];
        }


        std::vector<std::reference_wrapper<Cell>> getNeighborsCells(GridCoordinates coords);

        [[nodiscard]] std::vector<GridCoordinates> getNeighborsCoordinates(const GridCoordinates &coords) const;


        [[nodiscard]] inline Cell *getStartCell() const { return startCell; }

        [[nodiscard]] inline Cell *getEndCell() const { return endCell; }

        [[nodiscard]] inline GridCoordinates getStartCoordinates() const { return startCoordinates; }

        [[nodiscard]] inline GridCoordinates getEndCoordinates() const { return endCoordinates; }

        auto compareCells() {
            return [this](const GridCoordinates &a, const GridCoordinates &b) {
                return (*this)(a).getCost().totalCost() < (*this)(b).getCost().totalCost();
            };
        }

        inline void setSolved(){exitStatus = GridSolvedStatus::GRID_SOLVED;}
        inline void setUnsolvable(){exitStatus = GridSolvedStatus::GRID_UNSOLVABLE;}

    private:
        void init();

        uint32_t sizeX;
        uint32_t sizeY;
        std::vector<std::vector<Cell>> cells;

        Cell *startCell;
        Cell *endCell;

        GridCoordinates startCoordinates;
        GridCoordinates endCoordinates;

        GridSolvedStatus exitStatus;
    };


}