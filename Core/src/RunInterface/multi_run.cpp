#include "../../inc/RunInterface/runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy> &thisStrats) :
        RunnerParent(thisConfig),
        strats(thisStrats) {
    for (const auto &strat: strats) {
        solvers.push_back(Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid));
    }
    solverIterator = solvers.begin();
    currentSolver = solverIterator->get();

}

void RunInterface::MultiRun::nextStep() {
    if (!currentSolver->queueEmpty()) {
        currentSolver->nextStep();
    } else {
        grid.setUnsolvable();
    }
    if (grid.getEndCell()->getState() == GridGenerator::CellState::CELL_PATH) {
        grid.setSolved();
        handleFinishedGrid();
    }
}

void RunInterface::MultiRun::handleFinishedGrid() {
    solverIterator++;
     if (solverIterator != solvers.end()) {
        currentSolver = solverIterator->get();
        emit solverFinished();
    } else {
        if (gridIterator == config.iterations.value()) {
            emit demandNewConfiguration();
        } else {
            gridIterator++;
            solverIterator = solvers.begin();
            emit gridFinished();

        }
    }
}

void RunInterface::MultiRun::createNewGridWithCurrentConfig(bool reduceIteratorDueUnsolvable) {
    grid = GridGenerator::Grid(config.gridWidth,
                               config.gridHeight,
                               *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                       config.obstacleGenStrategy),
                               config.obstacleDensity, config.minStartEndDistance);
    for (const auto &strat: strats) {
        solvers.push_back(Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid));
    }
}

void RunInterface::MultiRun::nextRun() {
    while(grid.getStatus() == GridGenerator::GridSolvedStatus::GRID_UNSOLVED){
        nextStep();
    }
}

void RunInterface::MultiRun::onNewData(const RunInterface::RunGridConfig &thisConfig,
                                       const std::list<Pathfinder::PathfinderStrategy> &thisStrats) {
    config = thisConfig;
    strats = thisStrats;
}
