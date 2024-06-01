#include "../../inc/Pathfinder/pathfinding.h"

const std::map<Pathfinder::PathfinderStrategy, std::string>
        Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText =
        {{PathfinderStrategy::PATHFINDER_DIJKSTRA, "Dijkstra's Algorithm"},
         {PathfinderStrategy::PATHFINDER_A_STAR,   "A* Algorithm"}};

std::unique_ptr<Pathfinder::pathfindingParent> Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(
        PathfinderStrategy strat,
        GridGenerator::Grid &grid) {
    switch(strat){
        case PathfinderStrategy::PATHFINDER_A_STAR:
            return std::make_unique<AStarSolve>(grid);
        case PathfinderStrategy::PATHFINDER_DIJKSTRA:
            return std::make_unique<DijkstraSolve>(grid);
    }
    return nullptr;
}

void Pathfinder::pathfindingParent::registerCallback(Pathfinder::CallbackType callbackType,
                                                     const std::function<void()> &callback) {
    switch (callbackType) {
        case CallbackType::CALLBACK_AFTER_STEP:
            afterStepCallback.push_back(callback);
            break;
        case CallbackType::CALLBACK_AFTER_RUN:
            afterRunCallback.push_back(callback);
            break;
        case CallbackType::CALLBACK_BEFORE_STEP:
            beforeStepCallback.push_back(callback);
            break;
        case CallbackType::CALLBACK_BEFORE_RUN:
            beforeRunCallback.push_back(callback);
            break;
    }
}

void Pathfinder::pathfindingParent::triggerCallbacks(Pathfinder::CallbackType callbackType) {
    std::vector<std::function<void()>>* callbacks;
    switch (callbackType) {
        case CallbackType::CALLBACK_AFTER_STEP:
            callbacks = &afterStepCallback;
            break;
        case CallbackType::CALLBACK_AFTER_RUN:
            callbacks = &afterRunCallback;
            break;
        case CallbackType::CALLBACK_BEFORE_STEP:
            callbacks = &beforeStepCallback;
            break;
        case CallbackType::CALLBACK_BEFORE_RUN:
            callbacks = &beforeRunCallback;
            break;
    }
    for(const auto& callback:*callbacks){
        callback();
    }
}
