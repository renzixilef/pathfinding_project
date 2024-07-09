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
        setDisplayableStringForCurrentConfig(queueConfigItem);
        qInfo() << "STARTING: " << qPrintable(queueConfigItem.second);
        updateProgressBar(0);
        headlessSolveOneConfigNoWait(queueConfigItem);
        updateProgressBar(100);
        QThread::msleep(500);
        qInfo() << "FINISHED: " << qPrintable(queueConfigItem.second) << "\n";
        runQueue.pop();
    }
    headlessPrintEvaluation();
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

void Application::HeadlessRunner::headlessPrintEvaluation() {
    auto &stratToText =  Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText;
    qInfo() << "### EVALUATION ###";
    for (auto &evalPair: evalMap) {
        QString labelText = std::get<2>(evalPair.second) + "\n";
        labelText += "UNSOLVABLE GRIDS GENERATED: " + QString::number(std::get<1>(evalPair.second)) + "\n";
        for(auto& pathfinderEval: std::get<0>(evalPair.second)){
            labelText += "Pathfinder: " +
                         QString::fromStdString(stratToText.at(pathfinderEval.first)) + "\n";
            labelText+= generateEvalString(pathfinderEval.second, false);
        }
        qInfo() << qPrintable(labelText);
    }
}

QString Application::HeadlessRunner::generateEvalString(const auto &evalList, bool formatForGui) {
    QString evalString{};
    double absoluteSolvingUSeconds = 0;
    uint32_t totalClosedCells = 0;
    uint32_t totalVisitedCells = 0;
    uint32_t absoluteStepsTaken = 0;
    uint32_t numberOfGrids = evalList.size();

    for(const auto& pathfinderReturn: evalList){
        absoluteSolvingUSeconds += pathfinderReturn.solvingUSeconds;
        absoluteStepsTaken += pathfinderReturn.stepCount;
        totalClosedCells += pathfinderReturn.closedCells;
        totalVisitedCells += pathfinderReturn.visitedCells;
    }
    double avgSolvingMSeconds = (absoluteSolvingUSeconds/1000)/numberOfGrids;
    double avgUSecondsPerStep = absoluteSolvingUSeconds/absoluteStepsTaken;
    double absoluteSolvingSeconds = absoluteSolvingUSeconds/(1000*1000);
    double avgStepsTaken = static_cast<double>(absoluteStepsTaken)/numberOfGrids;
    if(formatForGui) {
        evalString += "<pre>      ABSOLUTE SOLVING TIME [s]: " + QString::number(absoluteSolvingSeconds) + "</pre>";
        evalString += "<pre>      AVG SOLVING TIME [ms]: " + QString::number(avgSolvingMSeconds) + "</pre>";
        evalString += "<pre>      AVG STEP TIME [us]: " + QString::number(avgUSecondsPerStep) + "</pre>";
        evalString += "<pre>      AVG STEP COUNT [-]: " + QString::number(avgStepsTaken) + "</pre>";
        evalString += "<pre>      TOTAL CLOSED CELLS [-]: " + QString::number(totalClosedCells) + "</pre>";
        evalString += "<pre>      TOTAL VISITED CELLS [-]: " + QString::number(totalVisitedCells) + "</pre><br/>";
    }else{
        evalString += "\tABSOLUTE SOLVING TIME [s]: " + QString::number(absoluteSolvingSeconds) + "\n";
        evalString += "\tAVG SOLVING TIME [ms]: " + QString::number(avgSolvingMSeconds) + "\n";
        evalString += "\tAVG STEP TIME [us]: " + QString::number(avgUSecondsPerStep) + "\n";
        evalString += "\tAVG STEP COUNT [-]: " + QString::number(avgStepsTaken) + "\n";
        evalString += "\tTOTAL CLOSED CELLS [-]: " + QString::number(totalClosedCells) + "\n";
        evalString += "\tTOTAL VISITED CELLS [-]: " + QString::number(totalVisitedCells) + "\n\n";
    }
    return evalString;

}

