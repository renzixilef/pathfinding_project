#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

#include "cell.h"
#include "obstacle_gen.h"

//TODO: documentation

namespace GridGenerator {

    class Grid {
    public:
        Grid(uint32_t sizeX, uint32_t sizeY, float obstacleDensity, ObstacleGenerator &generator) :
                sizeX(sizeX), sizeY(sizeY), cells(sizeX, std::vector<Cell>(sizeY)), start(nullptr), end(nullptr) {
            generator.generateObstacles(cells, obstacleDensity);
            //TODO: set start and end cell in init or in generateObstacles
            init();
        }

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





        inline Cell *getStart() { return start; }

        inline Cell *getEnd() { return start; }


    private:
        void init();

        uint32_t sizeX;
        uint32_t sizeY;
        std::vector<std::vector<Cell>> cells;
        Cell *start;
        Cell *end;
    };


}