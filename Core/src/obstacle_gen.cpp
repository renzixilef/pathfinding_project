#include <cstdint>
#include <algorithm>
#include <cmath>
#include <unordered_set>

#include "../inc/obstacle_gen.h"

//TODO: documentation
//TODO: implement always solvable grid generation via vector slicing and std::vector<Cell>.isblocked method
//TODO: set start and end cell in init or in generateObstacles

void GridGenerator::RandomObstacleGenerator::generateObstacles(GridGenerator::Grid &grid, float obstacleDensity) {
    using GridGenerator::GridCoordinate;
    std::unordered_set<GridCoordinate,
            decltype(&GridCoordinate::getHash)> allObstacleSet(0, GridCoordinate::getHash);
    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();
    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    for (uint32_t i = 0; i < (uint32_t) obstacleDensity * (sizeX * sizeY); ++i) {
        GridCoordinate thisCoord{distrX(gen), distrY(gen)};
        if (grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE) {
            grid(thisCoord).markObstacle();
            allObstacleSet.insert(thisCoord);
        } else {
            --i;
        }
    }
}

void GridGenerator::RandomWallLikeGenerator::generateObstacles(Grid &grid, float obstacleDensity) {
    using GridGenerator::GridCoordinate;
    std::unordered_set<GridCoordinate,
            decltype(&GridCoordinate::getHash)> allObstacleSet(0, GridCoordinate::getHash);
    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();
    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);

    uint64_t numberOfObstacleCells = (uint64_t) (sizeX * sizeY * obstacleDensity);
    for (uint64_t i = 0; i < numberOfObstacleCells;) {
        uint32_t startX = distrX(gen);
        uint32_t startY = distrY(gen);

        std::uniform_int_distribution<> distrDir(0, 1);
        bool isVertical = distrDir(gen);

        uint32_t maxLen = isVertical ? sizeX : sizeY;
        std::exponential_distribution<> distrLength(WALL_LENGTH_EXPONENTIAL_LAMBDA);

        uint32_t wallLength = std::min((uint32_t) (distrLength(gen) * maxLen), maxLen);

        for (uint32_t j = 0; j < wallLength; j++) {
            if (isVertical) {
                if (startX + j < sizeX) {
                    GridCoordinate thisCoord{startX+j, startY};
                    if (grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE) {
                        grid(thisCoord).markObstacle();
                        allObstacleSet.insert(thisCoord);
                        i++;
                    }
                }
            } else {
                if (startY + j < sizeY) {
                    GridCoordinate thisCoord{startX, startY+j};
                    if (grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE) {
                        grid(thisCoord).markObstacle();
                        allObstacleSet.insert(thisCoord);
                        i++;
                    }
                }
            }
        }
    }
}

void GridGenerator::DrunkenWalk::generateObstacles(Grid &grid, float obstacleDensity) {
    using GridGenerator::GridCoordinate;
    std::unordered_set<GridCoordinate,
            decltype(&GridCoordinate::getHash)> allObstacleSet(0, GridCoordinate::getHash);

    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    // mark all cells in the array as unwalkable
    for (uint32_t i = 0; i<sizeX; i++)
        for (uint32_t j= 0; j<sizeY; j++)
            grid(GridCoordinate(i, j)).markObstacle();

    // carve out paths using the DrunkenWalk algorithm

    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);

    uint32_t currentX = distrX(gen);
    uint32_t currentY = distrY(gen);
    uint64_t numObstacles = (uint64_t) (sizeX * sizeY * (1 - obstacleDensity));

    // if obstacleDensity is close to 1 this will take exponentially longer
    for (uint64_t i = 0; i < numObstacles; i++) {
        GridCoordinate thisCoord{currentX, currentY};
        if (grid(thisCoord).getState() == CellState::CELL_OBSTACLE) {
            grid(thisCoord).markOpen();
            allObstacleSet.erase(thisCoord);
        } else {
            i--;
        }
        uint8_t dx = distrX(gen) > (sizeX / 2) ? 1 : -1;
        uint8_t dy = distrY(gen) > (sizeY / 2) ? 1 : -1;

        currentX = (currentX + dx + sizeX) % sizeX;
        currentY = (currentY + dy + sizeY) % sizeY;
    }
}

void GridGenerator::PerlinNoise::generateObstacles(Grid &grid, float obstacleDensity) {
    using GridGenerator::GridCoordinate;
    std::unordered_set<GridCoordinate,
            decltype(&GridCoordinate::getHash)> allObstacleSet(0, GridCoordinate::getHash);

    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    for (uint32_t i = 0; i < sizeX; i++) {
        for (uint32_t j = 0; j < sizeY; j++) {
            double x = (double) (i / sizeX);
            double y = (double) (j / sizeY);

            double cellNoise = noise(x, y);
            if (cellNoise < obstacleDensity) {
                GridCoordinate thisCoord{i, j};
                grid(thisCoord).markObstacle();
                allObstacleSet.insert(thisCoord);
            }
        }
    }
}

double GridGenerator::PerlinNoise::noise(double x, double y) {
    using std::lerp;
    std::vector<uint8_t> permutation;
    permutation.resize(256);
    std::iota(permutation.begin(), permutation.end(), 0);
    std::shuffle(permutation.begin(), permutation.end(), gen);
    permutation.insert(permutation.end(), permutation.begin(), permutation.end());


    uint64_t xFloor = (uint64_t) std::floor(x) & 255, yFloor = (uint64_t) floor(y) & 255;

    x -= floor(x);
    y -= floor(y);

    double xFadeCurve = fade(x), yFadeCurve = fade(y);

    uint64_t A = permutation[xFloor] + yFloor, AA = permutation[A], AB = permutation[A + 1];
    uint64_t B = permutation[xFloor + 1] + yFloor, BA = permutation[B], BB = permutation[B + 1];
    double blend = lerp(yFadeCurve, lerp(xFadeCurve, grad(permutation[AA], x, y),
                                         grad(permutation[BA], x - 1, y)),
                        lerp(xFadeCurve, grad(permutation[AB], x, y - 1),
                             grad(permutation[BB], x - 1, y - 1)));
    return (blend + 1) / 2;
}

double GridGenerator::PerlinNoise::grad(uint32_t hash, double x, double y) {
    uint64_t h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
