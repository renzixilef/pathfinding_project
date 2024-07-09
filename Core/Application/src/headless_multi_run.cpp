#include "headless_multi_run.h"
#include <QDebug>
#include <QProcess>
#include <iostream>
#include <QThread>

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
    QString iterationsString = QString("IT: 1");
    if (configItem.gridConfig.iterations.has_value()) {
        iterationsString = QString("IT: %1").arg(configItem.gridConfig.iterations.value());
    }
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
        qInfo() << "STARTING: " << queueConfigItem.second;
        updateProgressBar(0);
        headlessSolveOneConfigNoWait(queueConfigItem);
        updateProgressBar(100);
        QThread::msleep(500);
        qInfo() << "FINISHED: " << queueConfigItem.second << "\n";
        //runQueue.pop();
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
        auto percentageDone = static_cast<uint16_t>((static_cast<double>(it) / runConfigIterations)*100);
        updateProgressBar(percentageDone);
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

void Application::HeadlessRunner::updateProgressBar(uint16_t percent) {
    uint32_t progressBarWidth = getConsoleWidth();

    std::string progressBar;
    auto pos = static_cast<uint32_t>(progressBarWidth * percent / 100.0);
    for (int i = 0; i < progressBarWidth; i++) {
        if (i < pos) progressBar += "=";
        else if (i == pos) progressBar += ">";
        else progressBar += " ";
    }
    std::cout << "\r" << "[" << progressBar << "]" << percent << "%" << std::flush;
    if (percent >= 100) std::cout << std::endl;
}

uint32_t Application::HeadlessRunner::getConsoleWidth() {
    QProcess proc;
#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    // Using "tput cols" to get terminal width on Unix-like systems
    proc.start("sh", QStringList() << "-c" << "tput cols");
#elif defined(Q_OS_WIN)
    // Using "mode con" to get terminal width on Windows
    proc.start("cmd.exe", QStringList() << "/C" << "mode con" << "| find \"Columns\"");
#endif
    proc.waitForFinished();
    QString widthStr = proc.readAllStandardOutput().trimmed();
#if defined(Q_OS_WIN)
    // On Windows, extract width after ":" symbol
        widthStr = widthStr.split(":").last().trimmed();
#endif
    return widthStr.toInt();
}

