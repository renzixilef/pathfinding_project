#include "grid.h"

#include "obstacle_gen.h"

#include <functional>
#include <fstream>

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
        //TODO: debug this mess
        std::pair<int8_t, int8_t> *nextDir = endCell->getParentIfDirPair();
        GridCoordinate nextCoordinate = endCoordinates;
        getEndCell()->markPath();
        while (true) {
            nextCoordinate = {static_cast<uint32_t>(static_cast<int64_t>(nextCoordinate.x) + nextDir->first),
                              static_cast<uint32_t>(static_cast<int64_t>(nextCoordinate.y) + nextDir->second)};
            Cell &thisCell = (*this)(nextCoordinate);
            thisCell.markPath();
            pathCellCount++;
            if (nextCoordinate == startCoordinates) break;
            if (thisCell.getParentIfDirPair() != nullptr) {
                nextDir = thisCell.getParentIfDirPair();
            }
        }
    }
}

void GridGenerator::Grid::resetGrid() {
    exitStatus = GridSolvedStatus::GRID_UNSOLVED;
    pathCellCount = 0;
    closedCellCount = 0;
    visitedCellCount = 0;
    for (auto &row: cells) {
        for (auto &cell: row) {
            if (cell.getState() != CellState::CELL_OBSTACLE) {
                cell = GridGenerator::Cell();
            }
        }
    }
    startCell = &(*this)(startCoordinates);
    endCell = &(*this)(endCoordinates);
}

void GridGenerator::Grid::serialize(const std::string &filename) const {
    std::ofstream outputStream(filename, std::ios::binary);

    outputStream.write(reinterpret_cast<const char *>(&sizeX), sizeof(sizeX));
    outputStream.write(reinterpret_cast<const char *>(&sizeY), sizeof(sizeY));
    outputStream.write(reinterpret_cast<const char *>(&startCoordinates.x), sizeof(startCoordinates.x));
    outputStream.write(reinterpret_cast<const char *>(&startCoordinates.y), sizeof(startCoordinates.y));
    outputStream.write(reinterpret_cast<const char *>(&endCoordinates.x), sizeof(endCoordinates.x));
    outputStream.write(reinterpret_cast<const char *>(&endCoordinates.y), sizeof(endCoordinates.y));
    outputStream.write(reinterpret_cast<const char *>(&exitStatus), sizeof(exitStatus));
    outputStream.write(reinterpret_cast<const char *>(&pathCellCount), sizeof(pathCellCount));
    outputStream.write(reinterpret_cast<const char *>(&closedCellCount), sizeof(closedCellCount));
    outputStream.write(reinterpret_cast<const char *>(&visitedCellCount), sizeof(visitedCellCount));

    for(const auto& cellVec : cells){
        for(const auto& cell: cellVec){
            auto state = static_cast<uint8_t>(cell.getState());
            outputStream.write(reinterpret_cast<const char*>(&state), sizeof(state));
        }
    }
    outputStream.close();
}

GridGenerator::Grid GridGenerator::Grid::deserialize(const std::string &filename) {
    std::ifstream inputStream(filename, std::ios::binary);

    uint32_t sizeX, sizeY;
    inputStream.read(reinterpret_cast<char*>(&sizeX), sizeof(sizeX));
    inputStream.read(reinterpret_cast<char *>(&sizeY), sizeof(sizeY));

    GridCoordinate startCoordinates{}, endCoordinates{};
    inputStream.read(reinterpret_cast<char*>(&startCoordinates.x), sizeof(startCoordinates.x));
    inputStream.read(reinterpret_cast<char*>(&startCoordinates.y), sizeof(startCoordinates.y));

    inputStream.read(reinterpret_cast<char*>(&endCoordinates.x), sizeof(endCoordinates.x));
    inputStream.read(reinterpret_cast<char*>(&endCoordinates.y), sizeof(endCoordinates.y));

    GridSolvedStatus exitStatus;
    inputStream.read(reinterpret_cast<char*>(&exitStatus), sizeof(exitStatus));

    uint32_t pathCellCount, closedCellCount, visitedCellCount;
    inputStream.read(reinterpret_cast<char *>(&pathCellCount), sizeof(pathCellCount));
    inputStream.read(reinterpret_cast<char *>(&closedCellCount), sizeof(closedCellCount));
    inputStream.read(reinterpret_cast<char *>(&visitedCellCount), sizeof(visitedCellCount));

    std::vector<std::vector<Cell>> cells(sizeX, std::vector<Cell>(sizeY));
    for(auto &cellVec : cells){
        for(auto& cell : cellVec){
            uint8_t state;
            inputStream.read(reinterpret_cast<char *>(&state), sizeof(state));
            switch(static_cast<CellState>(state)){
                case CellState::CELL_OBSTACLE:
                    cell.markObstacle();
                    break;
                case CellState::CELL_OPEN:
                    cell.markPath();
                    break;
                case CellState::CELL_VISITED:
                    cell.markVisited();
                    break;
                case CellState::CELL_CLOSED:
                    cell.markClosed();
                    break;
                case CellState::CELL_PATH:
                    cell.markPath();
                    break;
            }
        }
    }
    return {cells,
            startCoordinates, endCoordinates,
            exitStatus,
            pathCellCount, closedCellCount, visitedCellCount};
}


