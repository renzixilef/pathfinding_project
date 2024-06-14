#include "../../inc/RunInterface/runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy> &thisStrats) :
        RunnerParent(thisConfig),
        strats(thisStrats){
    for (const auto &strat: thisStrats) {
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
        emit gridFinished();
    } else {
        emit stepFinished();
    }
}

void RunInterface::MultiRun::handleFinishedGrid() {
    solverIterator++;
    if(solverIterator != solvers.end()){
        currentSolver = solverIterator->get();
    }else{
        solverIterator = solvers.begin();
        grid = GridGenerator::Grid(config.gridWidth,
                                   config.gridHeight,
                                   *GridGenerator::ObstacleGenStrategyParser::parseObstacleGenStrategy(
                                           config.obstacleGenStrategy),
                                   config.obstacleDensity, config.minStartEndDistance);
    }
}
