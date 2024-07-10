#pragma once

namespace GridGenerator {
    // Predefined standard values for wall like and perlin noise algorithm
    constexpr uint16_t WALL_LENGTH_EXPONENTIAL_LAMBDA = 15;
    constexpr double PERLIN_NOISE_SCALE = 10.0;

    // Predefined standard values for grid generation
    constexpr double DEFAULT_OBSTACLE_DENSITY = 0.3;
    constexpr double DEFAULT_START_END_DISTANCE = 0.5;
    constexpr uint32_t DEFAULT_GRID_WIDTH = 100;
    constexpr uint32_t DEFAULT_GRID_HEIGHT = 100;
    constexpr uint8_t DEFAULT_SOLVER = 1;
    constexpr uint32_t DEFAULT_ITERATIONS = 1;
    constexpr uint8_t HORIZONTAL_DISTANCE = 1;
    constexpr uint8_t VERTICAL_DISTANCE = 1;
}