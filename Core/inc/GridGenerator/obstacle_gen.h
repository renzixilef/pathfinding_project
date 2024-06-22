/**
 * @file obstacle_gen.h
 * @brief Implements functionality to generate various types of obstacles for a grid.
 */

#pragma once

#include <map>
#include <string>
#include <vector>
#include <random>
#include <memory>

#include "grid.h"
#include "define.h"

/**
 * @namespace GridGenerator
 * @brief Namespace for classes and functionalities related to grid generator.
 */
namespace GridGenerator {

    /**
     * @enum ObstacleGenStrategy
     * @brief This enum represents the obstacle generation strategies that are used by the grid.
     */
    enum class ObstacleGenStrategy {
        OBSTACLE_RANDOM = 1, /**< Random obstacle generation. */
        OBSTACLE_WALL_LIKE = 2, /**< Wall-like obstacle generation. */
        OBSTACLE_DRUNKEN_WALK = 3, /**< Drunken Walk obstacle generation. */
        OBSTACLE_PERLIN_NOISE = 4 /**< Perlin Noise obstacle generation. */
    };

    // Forward declaration to avoid compile-time errors
    class ObstacleGenerator;

    /**
     * @struct ObstacleGenStrategyParser
     * @brief This struct is responsible for parsing the obstacle generation strategy.
     */
    struct ObstacleGenStrategyParser {
        /** obstacleGenStrategyToDisplayableText is a static map that maps ObstacleGenStrategy values to text */
        static const std::map<ObstacleGenStrategy, std::string> obstacleGenStrategyToDisplayableText;

        /**
         * @fn parseObstacleGenStrategy;
         * @brief This function parses the ObstacleGenStrategy enum and returns unique pointer to a corresponding ObstacleGenerator.
         * @param strat The ObstacleGenStrategy to parse.
         * @return Unique pointer to ObstacleGenerator
         */
        static std::unique_ptr<ObstacleGenerator> parseObstacleGenStrategy(ObstacleGenStrategy strat);
    };

    /**
     * @class ObstacleGenerator
     * @brief This is a base class for generating obstacles in a grid.
     */
    class ObstacleGenerator {
    public:
        /**
         * @brief Default constructor for the ObstacleGenerator class. Initializes the random number generator.
         */
        ObstacleGenerator() : gen(rd()) {}

        /**
         * @brief Destructor for the ObstacleGenerator class.
         */
        virtual ~ObstacleGenerator() = default;

        /**
         * @fn virtual generateObstacles
         * @brief Pure virtual function to generate the obstacles.
         * @param grid The grid where obstacles are to be generated.
         * @param obstacleDensity The density of the obstacles that are to be generated.
         * @param minStartEndDistance The required minimum distance between the start and end nodes.
         */
        virtual void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) = 0;

    protected:
        /**
         * @fn setStartAndEndCoordinate
         * @brief method to set the start and end Coordinate for the grid based on walkable Vector
         * @param grid The grid where start and End should be set
         * @param minStartEndDistance The required minimum distance between the start and end nodes.
         * @return True if placement possible, false otherwise.
         */
        bool setStartAndEndCoordinate(GridGenerator::Grid &grid, float minStartEndDistance);

        /**
         * @fn isValidToMarkAsObstacle
         * @brief method to check if a certain GridCoordinate on the grid can be mark as an obstacle.
         * @param grid The grid where the check should be performed.
         * @param thisCoord The coordinate that gets checked
         * @return True if Coordinate can be marked, false otherwise.
         */
        static inline bool isValidToMarkAsObstacle(const Grid& grid, const GridCoordinate& thisCoord) {
            return grid(thisCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE &&
                    thisCoord != grid.getStartCoordinates() &&
                    thisCoord != grid.getEndCoordinates();
        }


        std::random_device rd; /**< rd is a random device */
        std::mt19937 gen; /**< gen is a random number generator */

    private:
        /**
         * @fn generateWalkableVector
         * @brief Method to generate a vector containing all the walkable GridCoordinate objects in a grid.
         * @param grid The grid where the action should be performed.
         * @return std::vector of walkable GridCoordinate objects
         */
        static std::vector<GridCoordinate> generateWalkableVector(const Grid &grid);
    };

    /**
     * @class RandomObstacleGenerator
     * @brief This class represents a random obstacle generator and it extends from the base class ObstacleGenerator.
     */
    class RandomObstacleGenerator : public ObstacleGenerator {
    public:
        /**
         * @brief Constructor for the RandomObstacleGenerator class. Uses the parent class ObstacleGenerator's constructor.
         */
        explicit RandomObstacleGenerator() : ObstacleGenerator() {}

        /**
         * @fn generateObstacles
         * @brief Function to generate random obstacles. Overrides the method from the base class
         * @param grid The grid where obstacles are to be generated.
         * @param obstacleDensity The density of the obstacles that are to be generated.
         * @param minStartEndDistance The required minimum distance between the start and end nodes.
         */
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;
    };

    /**
     * @class RandomWallLikeGenerator
     * @brief This class is an extension of the ObstacleGenerator class to generate wall-like random obstacles.
     */
    class RandomWallLikeGenerator : public ObstacleGenerator {
    public:
        /**
         * @brief A constructor for the RandomWallLikeGenerator class. Uses the parent class ObstacleGenerator's constructor.
         */
        explicit RandomWallLikeGenerator() : ObstacleGenerator() {}

        /**
         * @fn generateObstacles
         * @brief Function to generate wall-like random obstacles. Overrides the method from the base class.
         *
         * @param grid Reference to the Grid object where obstacles are to be generated.
         * @param obstacleDensity The density of the obstacles that are to be generated.
         * @param minStartEndDistance The required minimum distance between start and end nodes.
         */
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;
    };

    /**
     * @class PerlinNoise
     * @brief Derives from ObstacleGenerator class, this class generates Perlin noise based obstacles on a grid.
     */
    class PerlinNoise : public ObstacleGenerator {
    public:
        /**
         * @brief Constructor for the PerlinNoise class, which initializes permutation variable and calls the parent class constructor.
         */
        explicit PerlinNoise() : ObstacleGenerator() {
            permutation.resize(256);
            std::iota(permutation.begin(), permutation.end(), 0);
            std::shuffle(permutation.begin(), permutation.end(), gen);
            permutation.insert(permutation.end(), permutation.begin(), permutation.end());
        }

        /**
         * @fn void generateObstacles
         * @brief Function to generate perlin-noise obstacles. Overrides the method from the base class.
         * @param grid Reference to the Grid object where obstacles are to be generated.
         * @param obstacleDensity The density of the obstacles that are to be generated.
         * @param minStartEndDistance The required minimum distance between start and end nodes.
         */
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;

    private:
        /**
         * @fn double noise
         * @brief This function generates Perlin noise given the x and y coordinates.
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return The Perlin noise value.
         */
        double noise(double x, double y);

        /**
         * @fn fade
         * @brief This function applies the fade function to t.
         * @param t The input value.
         * @return The faded value of t.
         */
        static inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

        /**
         * @fn grad
         * @brief This function calculates the gradient at coordinates (x, y) based on the given hash.
         * @param hash The given hash.
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return The calculated gradient.
         */
        static double grad(uint32_t hash, double x, double y);

        std::vector<uint8_t> permutation; /**< permutation is a vector containing permutation operations. */
    };

    /**
     * @class DrunkenWalk
     * @brief This class derives from the ObstacleGenerator class, using a random walk strategy (also known as “drunken walk”) to place obstacles in the grid.
     */
    class DrunkenWalk : public ObstacleGenerator {
    public:
        /**
         * @brief Constructor for the DrunkenWalk class, which calls the parent class ObstacleGenerator's constructor.
         */
        explicit DrunkenWalk() : ObstacleGenerator() {}

        /**
         * @fn generateObstacles
         * @brief Function to generate drunken-walk obstacles. Overrides the method from the base class
         * @param grid Reference to the Grid object where obstacles are to be placed.
         * @param obstacleDensity Defines the density of the obstacles that will be generated.
         * @param minStartEndDistance The minimum required distance between the starting and ending nodes of the path.
         */
        void generateObstacles(Grid &grid, float obstacleDensity, float minStartEndDistance) override;
    };
}