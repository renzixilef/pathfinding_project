#include "../../inc/RunInterface/runner.h"
#include <memory>

RunInterface::SingleRun::SingleRun(const RunInterface::RunGridConfig &thisConfig,
                                   const Pathfinder::PathfinderStrategy &thisStrat) :
        RunnerParent(thisConfig),
        strat(thisStrat),
        solver(nullptr) {
    solver = Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid);

}

void RunInterface::SingleRun::start() {
    solver->markShortestPath();
}
