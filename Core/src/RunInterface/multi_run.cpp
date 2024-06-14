#include "../../inc/RunInterface/runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy> &thisStrats,
                                 bool shouldRepeatUnsolvables) :
        RunnerParent(thisConfig),
        strats(thisStrats),
        repeatUnsolvables(shouldRepeatUnsolvables) {
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
    if (grid.getStatus() == GridGenerator::GRID_SOLVED) {
        solverIterator++;
        if (solverIterator != solvers.end()) {
            currentSolver = solverIterator->get();
            emit solverFinished(currentSolver->getPerformanceMetric(),
                                RunnerReturnStatus::RETURN_NORMAL);
        } else {
            if (gridIterator == config.iterations.value()) {
                emit solverFinished(currentSolver->getPerformanceMetric(),
                                    RunnerReturnStatus::RETURN_LAST_GRID_DONE);

            } else {
                gridIterator++;
                emit solverFinished(currentSolver->getPerformanceMetric(),
                                    RunnerReturnStatus::RETURN_LAST_SOLVER_DONE);

            }
        }
    } else {
        if (!repeatUnsolvables) gridIterator++;
        emit solverFinished(std::nullopt, RunnerReturnStatus::RETURN_UNSOLVABLE);
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
}

void RunInterface::MultiRun::nextRun() {
    grid.resetGrid();
    while (grid.getStatus() == GridGenerator::GridSolvedStatus::GRID_UNSOLVED) {
        nextStep();
    }
}

void RunInterface::MultiRun::onNewData(const RunInterface::RunGridConfig &thisConfig,
                                       const std::list<Pathfinder::PathfinderStrategy> &thisStrats) {
    config = thisConfig;
    strats = thisStrats;
}
