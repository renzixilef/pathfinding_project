#include "GridGenerator/grid.h"

#include "GridGenerator/obstacle_gen.h"

#include <functional>


std::vector<std::reference_wrapper<GridGenerator::Cell>>
GridGenerator::Grid::getNeighborsCells(GridCoordinate coords) {
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

std::vector<GridGenerator::GridCoordinate>
GridGenerator::Grid::getNeighborsCoordinates(const GridGenerator::GridCoordinate& coords) const{
    std::vector<GridCoordinate> neighbors;

    std::vector<std::pair<int8_t, int8_t>> offsets = {
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
                    GridCoordinate{static_cast<uint32_t>(neighborRow), static_cast<uint32_t>(neighborCol)});
        }
    }
    return neighbors;
}

GridGenerator::Grid::Grid(uint32_t sizeX, uint32_t sizeY, ObstacleGenerator &generator, float obstacleDensity,
                          float minStartEndDistance):
        sizeX(sizeX),
        sizeY(sizeY),
        cells(sizeX, std::vector<Cell>(sizeY)),
        startCell(nullptr),
        endCell(nullptr),
        startCoordinates(GridCoordinate{}),
        endCoordinates(GridCoordinate{}),
        exitStatus(GridSolvedStatus::GRID_UNSOLVED){
    generator.generateObstacles((*this), obstacleDensity, minStartEndDistance);
}

void GridGenerator::Grid::markPathByParentCells() {
    Cell* nextCell = endCell;
    while(nextCell != nullptr) {
        nextCell->markPath();
        nextCell = nextCell->getParent();
    }
}

void GridGenerator::Grid::resetGrid() {
    for(auto& row: cells){
        for(auto& cell:row){
            if(cell.getState() != CellState::CELL_OBSTACLE){
                cell = GridGenerator::Cell();
            }
        }
    }
}


