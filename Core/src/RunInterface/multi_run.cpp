#include "../../inc/RunInterface/runner.h"

RunInterface::MultiRun::MultiRun(const RunInterface::RunGridConfig &thisConfig,
                                 const std::list<Pathfinder::PathfinderStrategy>& thisStrats,
                                 uint32_t thisIterations) :
                                 RunnerParent(thisConfig),
                                 strats(thisStrats),
                                 iterations(thisIterations){

}

void RunInterface::MultiRun::start() {

}
