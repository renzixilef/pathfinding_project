#pragma once

#include "cell.h"
#include <vector>

#define WALL_LENGTH_EXPONENTIAL_LAMBDA 10

namespace GridGenerator{
    enum class ObstacleGenStrategy{
        OBSTACLE_RANDOM,
        OBSTACLE_SPECIFIC
    };

    class ObstacleGenerator {
    public:
        virtual void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) = 0;
    };

    class RandomObstacleGenerator: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
        // place the implementation here
    };
    class WallObstacleGenerator: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
        // place the implementation here
    };

}