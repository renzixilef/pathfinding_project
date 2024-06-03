#include "../../inc/RunInterface/runner.h"
#include <memory>

RunInterface::SingleRun::SingleRun(const RunInterface::RunGridConfig &thisConfig,
                                   const Pathfinder::PathfinderStrategy &thisStrat) :
        RunnerParent(thisConfig),
        strat(thisStrat),
        solver(nullptr) {
    solver = Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid);

}

void RunInterface::SingleRun::nextStep() {
    if (!solver->queueEmpty()) {
        solver->nextStep();
    } else {
        grid.setUnsolvable();
    }
    if (grid.getEndCell()->getState() == GridGenerator::CellState::CELL_PATH) {
        grid.setSolved();
        emit gridFinished();
    } else {
        emit stepFinished();
    }
}

void RunInterface::SingleRun::onRunReset() {
    grid = GridGenerator::Grid(config.gridWith,
                               config.gridHeight,
                               *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                       config.obstacleGenStrategy),
                               config.obstacleDensity, config.minStartEndDistance);
    solver = Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid);
    emit stepFinished();
}
