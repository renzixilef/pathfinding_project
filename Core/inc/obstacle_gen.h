#pragma once

#include "cell.h"
#include <vector>

#define WALL_LENGTH_EXPONENTIAL_LAMBDA 15

namespace GridGenerator{
    enum class ObstacleGenStrategy{
        OBSTACLE_RANDOM,
        OBSTACLE_SPECIFIC
    };

    class ObstacleGenerator {
    public:
        ObstacleGenerator():gen(rd()){}
        virtual void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) = 0;
    protected:
        std::random_device rd;
        std::mt19937 gen;
    };

    class RandomObstacleGenerator: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
    };
    class RandomWallLikeGenerator: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
    };
    class PerlinSimplexNoise: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
    };
    class DrunkenWalk: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
    };
    class BinarySpacePartitioning: public ObstacleGenerator {
    public:
        void generateObstacles(std::vector<std::vector<Cell>>& cells, float obstacleDensity) override;
    };

}