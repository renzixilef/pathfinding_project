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

        [[nodiscard]] std::vector<GridCoordinates> getNeighborsCoordinates(GridCoordinates coords) const;


        [[nodiscard]] inline Cell *getStartCell() const { return startCell; }

        [[nodiscard]] inline Cell *getEndCell() const { return endCell; }

        [[nodiscard]] inline GridCoordinates getStartCoordinates() const { return startCoordinates; }

        [[nodiscard]] inline GridCoordinates getEndCoordinates() const { return endCoordinates; }


    private:
        void init();

        uint32_t sizeX;
        uint32_t sizeY;
        std::vector<std::vector<Cell>> cells;

        Cell *startCell;
        Cell *endCell;

        GridCoordinates startCoordinates;
        GridCoordinates endCoordinates;
    };


}