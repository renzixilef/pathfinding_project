#include "../../inc/RunInterface/runner.h"

RunInterface::SingleRun::SingleRun(const RunInterface::RunGridConfig &thisConfig,
                                   const Pathfinder::PathfinderStrategy& thisStrat) :
                                   RunnerParent(thisConfig),
                                   strat(thisStrat){

}

void RunInterface::SingleRun::start() {

}
