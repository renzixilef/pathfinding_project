#include <random>
#include <cstdint>
#include "../inc/obstacle_gen.h"



void GridGenerator::RandomObstacleGenerator::generateObstacles(std::vector<std::vector<Cell>> &cells,
                                                               float obstacleDensity) {
    uint32_t sizeX = cells.size();
    uint32_t sizeY = cells[0].size();
    std::uniform_int_distribution<uint32_t> distrX(0, sizeX-1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY-1);
    std::random_device rd;
    std::mt19937 gen(rd());
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

void GridGenerator::WallObstacleGenerator::generateObstacles(std::vector<std::vector<Cell>> &cells,
                                                             float obstacleDensity) {
    uint32_t sizeX = cells.size();
    uint32_t sizeY = cells[0].size();
    std::uniform_int_distribution<uint32_t> distrX(0, sizeX-1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY-1);
    std::random_device rd;
    std::mt19937 gen(rd());

    uint64_t numberOfObstacleCells =  (uint64_t)(sizeX*sizeY*obstacleDensity);
    for(uint32_t i = 0; i<numberOfObstacleCells;){
        uint32_t startX = distrX(gen);
        uint32_t startY = distrY(gen);

        std::uniform_int_distribution<> distrDir(0,1);
        bool isVertical = distrDir(gen);

        uint32_t maxLen = isVertical? sizeX:sizeY;
        std::exponential_distribution<> distrLength(WALL_LENGTH_EXPONENTIAL_LAMBDA);

        uint32_t wallLength = std::min(static_cast<uint32_t>(distrLength(gen)* maxLen), maxLen);

        for(uint32_t j=0; j<wallLength; j++){
            if(isVertical){
                if(startX+j <sizeX){
                    if (cells[startX + j][startY].getState() != GridGenerator::CellState::CELL_OBSTACLE) {
                        cells[startX + j][startY].markObstacle();
                        i++;
                    }
                }
            }else{
                if(startY+j<sizeY){
                    if (cells[startX][startY + j].getState() != GridGenerator::CellState::CELL_OBSTACLE) {
                        cells[startX][startY + j].markObstacle();
                        i++;
                    }
                }
            }
        }
    }
}
