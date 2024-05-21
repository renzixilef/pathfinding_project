#include <ctime>
#include <random>
#include <cstdint>
#include "../inc/obstacle_gen.h"


void GridGenerator::RandomObstacleGenerator::generateObstacles(std::vector<std::vector<Cell>> &cells,
                                                               float obstacleDensity) {
    std::srand(std::time(0));
    uint32_t sizeX = cells.size();
    uint32_t sizeY = cells[0].size();
    for (uint32_t i = 0; i < (uint32_t) obstacleDensity*(sizeX*sizeY); ++i)
    {
        uint32_t x = std::rand() % sizeX;
        uint32_t y = std::rand() % sizeY;
        if (cells[x][y].getState() != GridGenerator::CellState::CELL_OBSTACLE)
            cells[x][y].markObstacle();
        else
            --i;
    }
}