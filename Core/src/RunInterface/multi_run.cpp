#include "../../inc/RunInterface/runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy> &thisStrats) :
        RunnerParent(thisConfig),
        strats(thisStrats){
    for (const auto &strat: thisStrats) {
        solvers.push_back(Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(strat, grid));
    }

}

void RunInterface::MultiRun::nextStep() {
    /*
    for(uint32_t i = 0; i<iterations;i++){
        for(auto& solver: solvers){
            solver->markShortestPath();
            grid.resetGrid();
        }
    }
    */
}
