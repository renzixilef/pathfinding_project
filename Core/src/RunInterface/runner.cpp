#include "../inc/RunInterface/runner.h"

RunInterface::RunnerParent::RunnerParent(const RunInterface::RunGridConfig &thisConfig)
        : config(thisConfig),
          grid(GridGenerator::Grid(config.gridWith,
                                   config.gridHeight,
                                   *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                           config.obstacleGenStrategy),
                                   config.obstacleDensity, config.minStartEndDistance)) {
}


