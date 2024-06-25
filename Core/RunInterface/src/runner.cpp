#include "runner.h"

RunInterface::RunnerParent::RunnerParent(const RunInterface::RunGridConfig &thisConfig)
        : config(thisConfig),
          grid(GridGenerator::Grid(config.gridWidth,
                                   config.gridHeight,
                                   *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                           config.obstacleGenStrategy),
                                   config.obstacleDensity, config.minStartEndDistance)) {
}


