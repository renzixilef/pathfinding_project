#include "GridGenerator/grid.h"

#include "GridGenerator/obstacle_gen.h"

#include <functional>

const std::vector<std::pair<int8_t, int8_t>> GridGenerator::Grid::offsets = {
        {-1, 0},
        {1,  0},
        {0,  -1},
        {0,  1},  // Top, Bottom, Left, Right
        {-1, -1},
        {-1, 1},
        {1,  -1},
        {1,  1}  // Diagonals
};

std::vector<GridGenerator::GridCoordinate>
GridGenerator::Grid::getNeighborsCoordinates(const GridGenerator::GridCoordinate &coords) const {
    std::vector<GridCoordinate> neighbors;

    for (const auto &offset: offsets) {
        int64_t neighborRow = coords.x + offset.first;
        int64_t neighborCol = coords.y + offset.second;
        if (isInBounds(neighborRow, neighborCol)) {
            neighbors.push_back(
                    GridCoordinate{static_cast<uint32_t>(neighborRow), static_cast<uint32_t>(neighborCol)});
        }
    }
    return neighbors;
}

GridGenerator::Grid::Grid(uint32_t sizeX, uint32_t sizeY, ObstacleGenerator &generator, float obstacleDensity,
                          float minStartEndDistance) :
        sizeX(sizeX),
        sizeY(sizeY),
        cells(sizeX, std::vector<Cell>(sizeY)),
        startCell(nullptr),
        endCell(nullptr),
        startCoordinates(GridCoordinate{}),
        endCoordinates(GridCoordinate{}),
        exitStatus(GridSolvedStatus::GRID_UNSOLVED) {
    generator.generateObstacles((*this), obstacleDensity, minStartEndDistance);
}

void GridGenerator::Grid::markPathByParentCells(bool markByCellPointer) {
    if (markByCellPointer) {
        Cell *nextCell = endCell;
        while (nextCell != nullptr) {
            pathCellCount++;
            nextCell->markPath();
            nextCell = *nextCell->getParentIfCellPointer();
        }
    } else {
        std::pair<int8_t, int8_t> *nextDir = endCell->getParentIfDirPair();
        GridCoordinate nextCoordinate = endCoordinates;
        getEndCell()->markPath();
        while (true) {
            nextCoordinate = {static_cast<uint32_t>(static_cast<int64_t>(nextCoordinate.x) + nextDir->first),
                              static_cast<uint32_t>(static_cast<int64_t>(nextCoordinate.y) + nextDir->second)};
            Cell &thisCell = (*this)(nextCoordinate);
            thisCell.markPath();
            if (nextCoordinate == startCoordinates) break;
            if (thisCell.getParentIfDirPair() != nullptr) {
                nextDir = thisCell.getParentIfDirPair();
            }
        }
    }
}

void GridGenerator::Grid::resetGrid() {
    exitStatus = GridSolvedStatus::GRID_UNSOLVED;
    pathCellCount = 1;
    closedCellCount = 0;
    visitedCellCount = 0;
    for (auto &row: cells) {
        for (auto &cell: row) {
            if (cell.getState() != CellState::CELL_OBSTACLE) {
                cell = GridGenerator::Cell();
            }
        }
    }
}


