#include "runner.h"
#include <memory>

RunInterface::SingleRun::SingleRun(const RunInterface::SingleRunConfig &runConfig) :
        RunnerParent(runConfig.gridConfig),
        strat(runConfig.strat),
        solver(nullptr) {
    solver = Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid);

}

void RunInterface::SingleRun::nextStep() {
    if(atomicCloseFlag) return;
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
    grid = GridGenerator::Grid(config.gridWidth,
                               config.gridHeight,
                               *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                       config.obstacleGenStrategy),
                               config.obstacleDensity, config.minStartEndDistance);
    solver = Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid);
    emit stepFinished();
}

void RunInterface::SingleRun::onSerializeRequest(const std::string &filename) {
    grid.serialize(filename);
    emit saveDone();
}

void RunInterface::SingleRun::onStartEndChanged() {
    solver->reInit();
}
