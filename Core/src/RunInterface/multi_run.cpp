#include "../../inc/RunInterface/runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy> &thisStrats,
                                 uint32_t thisIterations) :
        RunnerParent(thisConfig),
        strats(thisStrats),
        iterations(thisIterations) {
    for (const auto &strat: thisStrats) {
        solvers.push_back(Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid));
    }

}

void RunInterface::MultiRun::start() {
    for(uint32_t i = 0; i<iterations;i++){
        for(auto& solver: solvers){
            solver->markShortestPath();
            grid.resetGrid();
        }
    }
}
