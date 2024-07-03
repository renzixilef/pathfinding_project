#include "runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy> &thisStrats) :
        RunnerParent(thisConfig),
        strats(thisStrats),
        repeatUnsolvables(thisConfig.repeatUnsolvables.value()) {
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
        handleFinishedSolver();
    }
    if (grid.getEndCell()->getState() == GridGenerator::CellState::CELL_PATH) {
        grid.setSolved();
        handleFinishedSolver();
    }
}

void RunInterface::MultiRun::handleFinishedSolver() {
    if (grid.getStatus() == GridGenerator::GridSolvedStatus::GRID_SOLVED) {
        solverIterator++;
        if (solverIterator != solvers.end()) {
            emit solverFinished(currentSolver->getPerformanceMetric(),
                                static_cast<int32_t>(RunnerReturnStatus::RETURN_NORMAL));
            currentSolver = solverIterator->get();
        } else {
            gridIterator++;
            if (gridIterator == config.iterations.value()) {
                emit solverFinished(currentSolver->getPerformanceMetric(),
                                    static_cast<int32_t>(RunnerReturnStatus::RETURN_LAST_GRID_DONE));

            } else {
                emit solverFinished(currentSolver->getPerformanceMetric(),
                                    static_cast<int32_t>(RunnerReturnStatus::RETURN_LAST_SOLVER_DONE));

            }
        }
    } else {
        if (!repeatUnsolvables) gridIterator++;
        emit solverFinished(currentSolver->getPerformanceMetric(),
                            static_cast<uint8_t>(RunnerReturnStatus::RETURN_UNSOLVABLE));
    }
}

void RunInterface::MultiRun::createNewGridWithCurrentConfig() {
    grid = GridGenerator::Grid(config.gridWidth,
                               config.gridHeight,
                               *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                       config.obstacleGenStrategy),
                               config.obstacleDensity, config.minStartEndDistance);
    solvers.clear();
    for (const auto &strat: strats) {
        solvers.push_back(Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid));
    }
    solverIterator = solvers.begin();
    currentSolver = solverIterator->get();
}

void RunInterface::MultiRun::nextRun() {
    grid.resetGrid();
    while (grid.getStatus() == GridGenerator::GridSolvedStatus::GRID_UNSOLVED && !atomicCloseFlag) {
        nextStep();
    }
}

void RunInterface::MultiRun::onNewData(const RunInterface::RunGridConfig &thisConfig,
                                       const std::list<Pathfinder::PathfinderStrategy> &thisStrats) {
    config = thisConfig;
    strats = thisStrats;
    repeatUnsolvables = config.repeatUnsolvables.value();
}

