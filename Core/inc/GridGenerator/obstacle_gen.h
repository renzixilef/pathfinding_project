#pragma once

#include <map>
#include <string>
#include <vector>
#include <random>
#include <memory>

#include "grid.h"

#define WALL_LENGTH_EXPONENTIAL_LAMBDA 15

//TODO: documentation

namespace GridGenerator {
    enum class ObstacleGenStrategy {
        OBSTACLE_RANDOM = 1,
        OBSTACLE_WALL_LIKE = 2,
        OBSTACLE_DRUNKEN_WALK = 3,
        OBSTACLE_PERLIN_NOISE = 4
    };

    struct ObstacleGenStrategyParser{
        static const std::map<ObstacleGenStrategy, std::string> obstacleGenStrategyToDisplayableText;
        static std::unique_ptr<ObstacleGenerator> parseObstacleGenStrategy(ObstacleGenStrategy strat);
    };


    class ObstacleGenerator {
    public:
        ObstacleGenerator() : gen(rd()) {}
        virtual ~ObstacleGenerator() = default;
        virtual void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) = 0;

    protected:
        std::random_device rd;
        std::mt19937 gen;
    };

    class RandomObstacleGenerator : public ObstacleGenerator {
    public:
        explicit RandomObstacleGenerator():ObstacleGenerator(){}
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;
    };

    class RandomWallLikeGenerator : public ObstacleGenerator {
    public:
        explicit RandomWallLikeGenerator():ObstacleGenerator(){}
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;
    };

    class PerlinNoise : public ObstacleGenerator {
    public:
        explicit PerlinNoise():ObstacleGenerator(){}
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;

    private:
        double noise(double x, double y);

        static inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

        static double grad(uint32_t hash, double x, double y);
    };

    class DrunkenWalk : public ObstacleGenerator {
    public:
        explicit DrunkenWalk():ObstacleGenerator(){}
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;
    };
}