#include <cstdint>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <memory>

#include "GridGenerator/obstacle_gen.h"

//TODO: documentation
//TODO: implement always solvable grid generation via vector slicing and std::vector<Cell>.isblocked method


const std::map<GridGenerator::ObstacleGenStrategy, std::string>
        GridGenerator::ObstacleGenStrategyParser::obstacleGenStrategyToDisplayableText =
        {{ObstacleGenStrategy::OBSTACLE_RANDOM,       "Random"},
         {ObstacleGenStrategy::OBSTACLE_WALL_LIKE,    "Wall like"},
         {ObstacleGenStrategy::OBSTACLE_DRUNKEN_WALK, "Drunken Walk"},
         {ObstacleGenStrategy::OBSTACLE_PERLIN_NOISE, "Perlin Noise"}};

std::unique_ptr<GridGenerator::ObstacleGenerator> GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy
        (GridGenerator::ObstacleGenStrategy strat) {
    switch (strat) {
        case ObstacleGenStrategy::OBSTACLE_RANDOM:
            return std::make_unique<GridGenerator::RandomObstacleGenerator>();
        case ObstacleGenStrategy::OBSTACLE_WALL_LIKE:
            return std::make_unique<GridGenerator::RandomWallLikeGenerator>();
        case ObstacleGenStrategy::OBSTACLE_DRUNKEN_WALK:
            return std::make_unique<GridGenerator::DrunkenWalk>();
        case ObstacleGenStrategy::OBSTACLE_PERLIN_NOISE:
            return std::make_unique<GridGenerator::PerlinNoise>();
    }
    return nullptr;
}


void GridGenerator::RandomObstacleGenerator::generateObstacles(GridGenerator::Grid &grid, float obstacleDensity,
                                                               float minStartEndDistance) {
    using GridGenerator::GridCoordinate;
    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);

    minStartEndDistance = minStartEndDistance * std::min(sizeX, sizeY);
    GridCoordinate startCoord{distrX(gen), distrY(gen)};
    grid.setStart(startCoord);
    GridCoordinate endCoord{};
    do {
        endCoord = GridCoordinate{distrX(gen), distrY(gen)};
    } while (startCoord.getAbsDistanceTo(endCoord) < minStartEndDistance);
    grid.setEnd(endCoord);

    for (uint32_t i = 0; i < static_cast<uint32_t>(obstacleDensity * static_cast<float>(sizeX * sizeY)); ++i) {
        GridCoordinate thisCoord{distrX(gen), distrY(gen)};
        if ((grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE) &&
            (thisCoord != startCoord) &&
            (thisCoord != endCoord)) {
            grid(thisCoord).markObstacle();
        } else {
            --i;
        }
    }

}

void GridGenerator::RandomWallLikeGenerator::generateObstacles(Grid &grid, float obstacleDensity,
                                                               float minStartEndDistance) {
    using GridGenerator::GridCoordinate;

    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();
    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);

    minStartEndDistance = minStartEndDistance * std::min(sizeX, sizeY);
    GridCoordinate startCoord{distrX(gen), distrY(gen)};
    grid.setStart(startCoord);
    GridCoordinate endCoord{};
    do {
        endCoord = GridCoordinate{distrX(gen), distrY(gen)};
    } while (startCoord.getAbsDistanceTo(endCoord) < minStartEndDistance);
    grid.setEnd(endCoord);

    uint64_t numberOfObstacleCells = (sizeX * sizeY * obstacleDensity);
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
                    GridCoordinate thisCoord{startX + j, startY};
                    if (grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE &&
                        (thisCoord != startCoord) &&
                        (thisCoord != endCoord)) {
                        grid(thisCoord).markObstacle();
                        i++;
                    }
                }
            } else {
                if (startY + j < sizeY) {
                    GridCoordinate thisCoord{startX, startY + j};
                    if (grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE &&
                        (thisCoord != startCoord) &&
                        (thisCoord != endCoord)) {
                        grid(thisCoord).markObstacle();
                        i++;
                    }
                }
            }
        }
    }
}

void GridGenerator::DrunkenWalk::generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) {
    using GridGenerator::GridCoordinate;
    std::unordered_set<GridCoordinate,
            decltype(&GridCoordinate::getHash)> allWalkableCoordinateSet(0, GridCoordinate::getHash);

    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    // mark all cells in the array as unwalkable
    for (uint32_t i = 0; i < sizeX; i++)
        for (uint32_t j = 0; j < sizeY; j++)
            grid(GridCoordinate(i, j)).markObstacle();

    // carve out paths using the DrunkenWalk algorithm

    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);
    std::uniform_int_distribution<uint8_t> distrDir(1, 10);

    uint32_t currentX = distrX(gen);
    uint32_t currentY = distrY(gen);
    uint64_t numObstacles = (uint64_t) (sizeX * sizeY * (1 - obstacleDensity));

    // if obstacleDensity is close to 1 this will take exponentially longer
    for (uint64_t i = 0; i < numObstacles; i++) {
        GridCoordinate thisCoord{currentX, currentY};
        if (grid(thisCoord).getState() == CellState::CELL_OBSTACLE) {
            grid(thisCoord).markOpen();
            allWalkableCoordinateSet.insert(thisCoord);
        } else {
            i--;
        }
        uint8_t direction = distrDir(gen);

        uint32_t oldX = currentX;
        uint32_t oldY = currentY;

        int8_t dx = distrX(gen) > (sizeX / 2) ? 1 : -1;
        int8_t dy = distrY(gen) > (sizeY / 2) ? 1 : -1;

        if (direction <= 2) {
            currentX = (currentX + dx + sizeX) % sizeX;
            currentY = (currentY + dy + sizeY) % sizeY;
        } else {
            if (direction <= 6) currentY = (currentY + dy + sizeY) % sizeY;
            else currentX = (currentX + dx + sizeX) % sizeX;
        }
        if (oldX == currentX && oldY == currentY) {
            if (direction <= 6) currentX = (currentX + dx + sizeX) % sizeX;
            else currentY = (currentY + dy + sizeY) % sizeY;
        }

    }

    minStartEndDistance = minStartEndDistance * std::min(sizeX, sizeY);
    std::uniform_int_distribution<uint32_t> distrWalkableSet(0, allWalkableCoordinateSet.size() - 1);
    GridCoordinate startCoord{};
    GridCoordinate endCoord{};
    do {
        uint32_t randomIndexStart = distrWalkableSet(gen);
        auto iteratorStart = allWalkableCoordinateSet.begin();
        std::advance(iteratorStart, randomIndexStart);
        startCoord = *iteratorStart;
        for (uint32_t i = 0; i <= allWalkableCoordinateSet.size() / 5; i++) {
            uint32_t randomIndexEnd = distrWalkableSet(gen);
            auto iteratorEnd = allWalkableCoordinateSet.begin();
            std::advance(iteratorEnd, randomIndexEnd);
            endCoord = *iteratorEnd;
            if (startCoord.getAbsDistanceTo(endCoord) < minStartEndDistance) break;
        }
    } while (startCoord.getAbsDistanceTo(endCoord) < minStartEndDistance);
    grid.setStart(startCoord);
    grid.setEnd(endCoord);
}

void GridGenerator::PerlinNoise::generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) {
    using GridGenerator::GridCoordinate;

    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);

    uint32_t offsetX = distrX(gen);
    uint32_t offsetY = distrY(gen);

    minStartEndDistance = minStartEndDistance * std::min(sizeX, sizeY);
    GridCoordinate startCoord{distrX(gen), distrY(gen)};
    grid.setStart(startCoord);
    GridCoordinate endCoord{};
    do {
        endCoord = GridCoordinate{distrX(gen), distrY(gen)};
    } while (startCoord.getAbsDistanceTo(endCoord) < minStartEndDistance);
    grid.setEnd(endCoord);

    std::vector<double> noiseValues;
    for (uint32_t x = 0; x < sizeX; x++) {
        for (uint32_t y = 0; y < sizeY; y++) {
            double cellNoise = noise(x / PERLIN_NOISE_SCALE + offsetX, y / PERLIN_NOISE_SCALE + offsetY);
            noiseValues.push_back(cellNoise);
        }
    }

    std::sort(noiseValues.begin(), noiseValues.end());
    double obstacleDensityThreshold = noiseValues.at(
            static_cast<size_t>(static_cast<double>(1 - obstacleDensity) * sizeX * sizeY));


    for (uint32_t x = 0; x < sizeX; x++) {
        for (uint32_t y = 0; y < sizeY; y++) {

            double cellNoise = noise(x / PERLIN_NOISE_SCALE + offsetX, y / PERLIN_NOISE_SCALE + offsetY);
            if (cellNoise>obstacleDensityThreshold) {
                GridCoordinate thisCoord{x, y};
                if (thisCoord != startCoord && thisCoord != endCoord) {
                    grid(thisCoord).markObstacle();
                }
            }
        }
    }
}

double GridGenerator::PerlinNoise::noise(double x, double y) {
    using std::lerp;

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

