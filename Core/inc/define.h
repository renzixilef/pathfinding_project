#pragma once

#include <QMetaType>
#include <Pathfinder/pathfinding.h>
#include <RunInterface/runner.h>


Q_DECLARE_METATYPE(Pathfinder::PathfinderPerformanceMetric)

Q_DECLARE_METATYPE(RunInterface::RunGridConfig)

Q_DECLARE_METATYPE(Pathfinder::PathfinderStrategy)

Q_DECLARE_METATYPE(int32_t)


// Predefined standard values for wall like and perlin noise algorithm
#define WALL_LENGTH_EXPONENTIAL_LAMBDA 15
#define PERLIN_NOISE_SCALE 10.0

// Predefined standard values for obstacle density and start-end distance
#define STANDARD_OBSTACLE_DENSITY 0.3
#define STANDARD_START_END_DISTANCE 0.5
#define HORIZONTAL_DISTANCE 1
#define VERTICAL_DISTANCE 1