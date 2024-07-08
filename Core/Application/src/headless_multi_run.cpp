#include "headless_multi_run.h"

QString Application::HeadlessRunner::generateConfigQString(const RunInterface::MultiRunConfig &configItem) {
    QString gridWidthHeightString = QString("%1 x %2")
            .arg(configItem.gridConfig.gridHeight)
            .arg(configItem.gridConfig.gridWidth);
    QString obstacleGenString = QString::fromStdString(
            GridGenerator::ObstacleGenStrategyParser::obstacleGenStrategyToDisplayableText.at(
                    configItem.gridConfig.obstacleGenStrategy));
    QString obstacleDensityString = QString("OD: %1%").arg(configItem.gridConfig.obstacleDensity * 100.0);
    QString minStartEndDistanceString = QString("SE: %1%").arg(configItem.gridConfig.minStartEndDistance * 100.0);
    QString pathfinderString = "[";
    for (auto iter = configItem.strats.begin(); iter != configItem.strats.end(); iter++) {
        if (iter != configItem.strats.begin()) {
            pathfinderString += ", ";
        }
        if (Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.find(*iter) !=
            Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.end()) {
            pathfinderString += QString::fromStdString(
                    Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.at(*iter));
        }
    }
    pathfinderString += "]";

    QString iterationsString = QString("IT: %1").arg(configItem.gridConfig.iterations.value());
    QString itemText = QString("%1, %2, %3, %4, %5, %6")
            .arg(gridWidthHeightString,
                 obstacleGenString,
                 obstacleDensityString,
                 minStartEndDistanceString,
                 pathfinderString,
                 iterationsString);

    return itemText;
}

void Application::HeadlessRunner::headlessSolveAllNoWait() {
    while (!runQueue.empty()) {
        auto queueConfigItem = runQueue.front();
        //TODO: print that config xy started
        headlessSolveOneConfigNoWait(queueConfigItem);
        //TODO: print that config xy finished
        runQueue.pop();
    }
    //TODO: move to evaluation
}

void Application::HeadlessRunner::headlessSolveOneConfigNoWait(
        std::pair<RunInterface::MultiRunConfig, QString> &itemConfig) {
    using obstacleParser = GridGenerator::ObstacleGenStrategyParser;
    using solverParser = Pathfinder::PathfinderStrategyParser;
    RunInterface::MultiRunConfig config = itemConfig.first;
    uint32_t runConfigIterations = config.gridConfig.iterations.has_value() ?
                                   config.gridConfig.iterations.value() : 1;
    for (int it = 0; it < runConfigIterations; it++) {
        //TODO: updateProgress
        bool validSolution = false;
        while (!validSolution) {
            GridGenerator::Grid thisGrid(config.gridConfig.gridWidth, config.gridConfig.gridHeight,
                                         *obstacleParser::parseObstacleGenStrategy(
                                                 config.gridConfig.obstacleGenStrategy),
                                         config.gridConfig.obstacleDensity, config.gridConfig.minStartEndDistance);

            for (const auto &strat: config.strats) {
                auto solver = solverParser::parsePathfinderStrategy(strat, thisGrid);
                solver->solveNoWait();
                if (thisGrid.getStatus() == GridGenerator::GridSolvedStatus::GRID_UNSOLVABLE) {
                    if (config.gridConfig.repeatUnsolvables.has_value() &&
                        !config.gridConfig.repeatUnsolvables.value()) {
                        validSolution = true;
                    }
                    break;
                } else {
                    pushBackPathfinderExitForCurrentConfig(solver->getPerformanceMetric(), itemConfig);
                    thisGrid.resetGrid();
                    validSolution = true;
                }
            }
        }
    }
}

