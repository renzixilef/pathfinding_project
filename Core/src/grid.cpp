#include "../inc/grid.h"

#include <functional>


std::vector<std::reference_wrapper<GridGenerator::Cell>>
GridGenerator::Grid::getNeighborsCells(GridCoordinates coords) {
    std::vector<std::reference_wrapper<Cell>> neighbors;

    std::vector<std::pair<uint8_t, uint8_t>> offsets = {
            {-1, 0},
            {1,  0},
            {0,  -1},
            {0,  1},  // Top, Bottom, Left, Right
            {-1, -1},
            {-1, 1},
            {1,  -1},
            {1,  1}  // Diagonals
    };

    for (const auto &offset: offsets) {
        int64_t neighborRow = coords.x + offset.first;
        int64_t neighborCol = coords.y + offset.second;
        if (neighborRow >= 0 && neighborRow < sizeX && neighborCol >= 0 && neighborCol < sizeY) {
            neighbors.push_back(std::ref(cells[neighborRow][neighborCol]));
        }
    }
    return neighbors;
}

std::vector<GridGenerator::GridCoordinates>
GridGenerator::Grid::getNeighborsCoordinates(GridGenerator::GridCoordinates coords) const{
    std::vector<GridCoordinates> neighbors;

    std::vector<std::pair<uint8_t, uint8_t>> offsets = {
            {-1, 0},
            {1,  0},
            {0,  -1},
            {0,  1},  // Top, Bottom, Left, Right
            {-1, -1},
            {-1, 1},
            {1,  -1},
            {1,  1}  // Diagonals
    };

    for (const auto &offset: offsets) {
        int64_t neighborRow = coords.x + offset.first;
        int64_t neighborCol = coords.y + offset.second;
        if (neighborRow >= 0 && neighborRow < sizeX && neighborCol >= 0 && neighborCol < sizeY) {
            neighbors.push_back(
                    GridCoordinates{static_cast<uint32_t>(neighborRow), static_cast<uint32_t>(neighborCol)});
        }
    }
    return neighbors;
}

GridGenerator::Grid::Grid(uint32_t sizeX, uint32_t sizeY, float obstacleDensity, ObstacleGenerator &generator) :
        sizeX(sizeX),
        sizeY(sizeY),
        cells(sizeX, std::vector<Cell>(sizeY)),
        startCell(nullptr),
        endCell(nullptr),
        startCoordinates(GridCoordinates{}),
        endCoordinates(GridCoordinates{}) {
    generator.generateObstacles(cells, obstacleDensity);
    //TODO: set start and end cell in init or in generateObstacles
    init();
}


