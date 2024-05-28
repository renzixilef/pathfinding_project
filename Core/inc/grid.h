#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

#include "cell.h"
#include "obstacle_gen.h"

//TODO: documentation

namespace GridGenerator {
    struct GridCoordinates{
        uint32_t x;
        uint32_t y;
    };


    class Grid {
    public:
        Grid(uint32_t sizeX, uint32_t sizeY, float obstacleDensity, ObstacleGenerator &generator);

        Cell& operator() (size_t row, size_t col) {
            if(row >= sizeX || col >= sizeY)
                throw std::out_of_range("Grid indices out of range");
            return cells[row][col];
        }

        const Cell& operator() (size_t row, size_t col) const {
            if(row >= sizeX || col >= sizeY)
                throw std::out_of_range("Grid indices out of range");
            return cells[row][col];
        }


        std::vector<std::reference_wrapper<Cell>> getNeighbors(size_t row, size_t col);

        inline Cell *getStartCell() {return startCell;}
        inline Cell *getEndCell() {return endCell;}

        inline GridCoordinates getStartCoordinates(){return startCoordinates;}
        inline GridCoordinates getEndCoordinates(){return endCoordinates;}


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