#include "obstacle_gen.h"

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

    setStartAndEndCoordinate(grid, minStartEndDistance);

    for (uint32_t i = 0; i < static_cast<uint32_t>(obstacleDensity * static_cast<float>(sizeX * sizeY)); ++i) {
        GridCoordinate thisCoord{distrX(gen), distrY(gen)};
        if (isValidToMarkAsObstacle(grid, thisCoord)) {
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

    setStartAndEndCoordinate(grid, minStartEndDistance);

    auto numberOfObstacleCells = static_cast<uint64_t>(static_cast<double>(sizeX * sizeY) * obstacleDensity);

    for (uint64_t i = 0; i < numberOfObstacleCells;) {
        uint32_t startX = distrX(gen);
        uint32_t startY = distrY(gen);

        std::uniform_int_distribution<> distrDir(0, 1);
        bool isVertical = distrDir(gen);

        uint32_t maxLen = isVertical ? sizeX : sizeY;
        std::exponential_distribution<> distrLength(WALL_LENGTH_EXPONENTIAL_LAMBDA);

        uint32_t wallLength = std::min((uint32_t) (distrLength(gen) * maxLen), maxLen);

        for (uint32_t j = 0; j < wallLength; j++) {
            bool withinGridSize = isVertical ? startX + j < sizeX : startY + j < sizeY;
            if (!withinGridSize) continue;
            GridCoordinate thisCoord = isVertical ? GridCoordinate{startX + j, startY} :
                                       GridCoordinate{startX, startY + j};
            if (isValidToMarkAsObstacle(grid, thisCoord)) {
                grid(thisCoord).markObstacle();
                i++;
            }
        }
    }
}

void GridGenerator::DrunkenWalk::generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) {
    using GridGenerator::GridCoordinate;
    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    auto numObstacles = static_cast<uint64_t>(sizeX * sizeY * static_cast<double>(1 - obstacleDensity));

    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);
    std::uniform_int_distribution<uint8_t> distrDir(1, 10);

    do {
        // mark all cells in the array as unwalkable
        for (uint32_t i = 0; i < sizeX; i++)
            for (uint32_t j = 0; j < sizeY; j++)
                grid(GridCoordinate(i, j)).markObstacle();

        // carve out paths using the DrunkenWalk algorithm

        uint32_t currentX = distrX(gen);
        uint32_t currentY = distrY(gen);

        // if obstacleDensity is close to 1 this will take exponentially longer
        for (uint64_t i = 0; i < numObstacles; i++) {
            GridCoordinate thisCoord{currentX, currentY};
            if (grid(thisCoord).getState() == CellState::CELL_OBSTACLE) {
                grid(thisCoord).markOpen();
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
    } while (!setStartAndEndCoordinate(grid, minStartEndDistance));
}

void GridGenerator::PerlinNoise::generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) {
    using GridGenerator::GridCoordinate;

    uint32_t sizeX = grid.getSizeX();
    uint32_t sizeY = grid.getSizeY();

    std::uniform_int_distribution<uint32_t> distrX(0, sizeX - 1);
    std::uniform_int_distribution<uint32_t> distrY(0, sizeY - 1);

    uint32_t offsetX = distrX(gen);
    uint32_t offsetY = distrY(gen);

    setStartAndEndCoordinate(grid, minStartEndDistance);

    std::vector<std::vector<double>> gridNoiseValues(sizeX, std::vector<double>(sizeY));
    std::vector<double> noiseValues;
    for (uint32_t x = 0; x < sizeX; x++) {
        for (uint32_t y = 0; y < sizeY; y++) {
            double cellNoise = noise(x / PERLIN_NOISE_SCALE + offsetX, y / PERLIN_NOISE_SCALE + offsetY);
            noiseValues.push_back(cellNoise);
            gridNoiseValues.at(x).at(y) = cellNoise;
        }
    }

    std::sort(noiseValues.begin(), noiseValues.end());
    double obstacleDensityThreshold = noiseValues.at(
            static_cast<size_t>(static_cast<double>(1 - obstacleDensity) * sizeX * sizeY));

    for (uint32_t x = 0; x < sizeX; x++) {
        for (uint32_t y = 0; y < sizeY; y++) {
            if (gridNoiseValues.at(x).at(y) > obstacleDensityThreshold) {
                GridCoordinate thisCoord{x, y};
                if (isValidToMarkAsObstacle(grid, thisCoord)) {
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

std::vector<GridGenerator::GridCoordinate> GridGenerator::ObstacleGenerator::generateWalkableVector(const Grid &grid) {
    std::vector<GridCoordinate> allWalkableCoordinateVector;
    for (uint32_t x = 0; x < grid.getSizeX(); x++) {
        for (uint32_t y = 0; y < grid.getSizeY(); y++) {
            GridCoordinate coord{x, y};
            if (grid(coord).getState() != CellState::CELL_OBSTACLE) {
                allWalkableCoordinateVector.push_back(coord);
            }
        }
    }
    return allWalkableCoordinateVector;
}

bool GridGenerator::ObstacleGenerator::setStartAndEndCoordinate(GridGenerator::Grid &grid, float minStartEndDistance) {
    auto walkableVector = generateWalkableVector(grid);
    minStartEndDistance *= static_cast<float>(std::min(grid.getSizeX(), grid.getSizeY()));

    std::uniform_int_distribution<uint32_t> distrWalkableSet(0, walkableVector.size() - 1);

    while (!walkableVector.empty()) {
        uint32_t randomIndexStart = distrWalkableSet(gen);
        GridCoordinate startCoord = walkableVector[randomIndexStart];

        std::swap(walkableVector[randomIndexStart], walkableVector.back());
        walkableVector.pop_back();
        distrWalkableSet = std::uniform_int_distribution<uint32_t>(0, walkableVector.size() - 1);


        std::vector<GridCoordinate> validEndPoints;
        for (const auto &endCoord: walkableVector) {
            if (startCoord.getAbsDistanceTo(endCoord) > minStartEndDistance) {
                validEndPoints.push_back(endCoord);
            }
        }
        if (!validEndPoints.empty()) {
            std::uniform_int_distribution<uint32_t> distrEndPoints(0, validEndPoints.size() - 1);
            uint32_t randomIndexEnd = distrEndPoints(gen);
            GridCoordinate endCoord = validEndPoints[randomIndexEnd];
            grid.setStart(startCoord);
            grid.setEnd(endCoord);
            return true;
        }
    }
    return false;

}
