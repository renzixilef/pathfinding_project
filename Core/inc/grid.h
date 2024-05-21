#pragma once

#include <vector>
#include <cstdint>
#include "cell.h"
#include "obstacle_gen.h"

namespace GridGenerator {

    class Grid {
    public:
        Grid(uint32_t size, float obstacleDensity, ObstacleGenerator& generator) : size(size), cells(size, std::vector<Cell>(size)), start(nullptr),
                                              end(nullptr) {
            generator.generateObstacles(cells, obstacleDensity);
            init();
        }

        inline Cell* getStart(){return start;}
        inline Cell* getEnd(){return start;}

    private:
        void init();
        uint32_t size;
        std::vector<std::vector<Cell>> cells;
        Cell *start;
        Cell *end;
    };


}