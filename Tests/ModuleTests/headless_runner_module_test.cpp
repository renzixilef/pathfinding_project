#include <gtest/gtest.h>
#include "runner.h"
#include "pathfinding.h"
#include "grid.h"
#include "headless_multi_run.h"

TEST(Application, HeadlessRunnerModuleTest) {
    std::queue<std::pair<RunInterface::MultiRunConfig, QString>> runQueue;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> distWidth(50, 500);
    std::uniform_int_distribution<uint16_t> distHeight(50, 500);
    std::uniform_real_distribution<float> distDensity(0.2, 0.7);
    std::uniform_real_distribution<float> distDistance(0.5, 0.9);
    std::uniform_int_distribution<uint8_t> distObstacleStrategy(1, 4);
    std::uniform_int_distribution<uint16_t> distIterations(20, 100);
    std::vector<Pathfinder::PathfinderStrategy> solverStrats{Pathfinder::PathfinderStrategy::PATHFINDER_DIJKSTRA,
                                                             Pathfinder::PathfinderStrategy::PATHFINDER_JUMP_POINT_SEARCH,
                                                             Pathfinder::PathfinderStrategy::PATHFINDER_A_STAR};
    std::uniform_int_distribution<int32_t> distSize(1, static_cast<int32_t>(solverStrats.size()) - 1);


    for (uint32_t it = 0; it < 10; it++) {
        std::shuffle(solverStrats.begin(), solverStrats.end(), gen);
        RunInterface::RunGridConfig config{distWidth(gen), distHeight(gen),
                                             distDensity(gen), distDistance(gen),
                                             static_cast<GridGenerator::ObstacleGenStrategy>(distObstacleStrategy(gen)),
                                             distIterations(gen), false};
        std::list<Pathfinder::PathfinderStrategy> stratSublist(solverStrats.begin(),
                                                               solverStrats.begin() + distSize(gen));
        RunInterface::MultiRunConfig multiConfig{config, stratSublist};
        runQueue.emplace(multiConfig, Application::HeadlessRunner::generateConfigQString(multiConfig));
    }
    Application::HeadlessRunner runner(runQueue);
    ASSERT_NO_THROW(runner.headlessSolveAllNoWait());
}

