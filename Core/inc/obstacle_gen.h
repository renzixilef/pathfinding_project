#pragma once

#include "cell.h"
#include <vector>

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

}