#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "command_line_parser.h"
#include "gui_main_window.h"

void headlessSolveAllNoWait(std::list<RunInterface::MultiRunConfig> &configList) {
    using obstacleParser = GridGenerator::ObstacleGenStrategyParser;
    using EvalMapType = std::map<RunInterface::RunGridConfig, std::tuple<std::unordered_map<
            Pathfinder::PathfinderStrategy, std::list<Pathfinder::PathfinderPerformanceMetric>>, uint32_t, QString>>;
    EvalMapType evalMap;
    for (const auto & config : configList) {
        GridGenerator::Grid thisGrid(config.gridConfig.gridWidth, config.gridConfig.gridHeight,
                                     *obstacleParser::parseObstacleGenStrategy(config.gridConfig.obstacleGenStrategy),
                                     config.gridConfig.obstacleDensity, config.gridConfig.minStartEndDistance);
        for(const auto& pathfinder : config.strats){
            auto solver = Pathfinder::PathfinderStrategyParser::parsePathfinderStrategy(pathfinder, thisGrid);
            solver->solveNoWait();
            if(thisGrid.getStatus() == GridGenerator::GridSolvedStatus::GRID_UNSOLVABLE) break;
            else{
                std::get<2>(solver->getPerformanceMetric();
                thisGrid.resetGrid();
            }
        }
    }
}

int main(int argc, char *argv[]) {
    QApplication pathfindingApp(argc, argv);
    QApplication::setApplicationName("Pathfinding");
    QApplication::setApplicationVersion("1.0");

    auto &parser = Application::PathfindingCommandParser::getInstance();


    parser.process(pathfindingApp);
    auto parserInputValid = parser.inputOptionsValid();

    if (!parserInputValid.first) {
        qCritical() << "Incompatible flag set: " << parserInputValid.second << "\nExiting the application!";
        return 1;
    }


    if (parser.getUIType() == Application::UIType::UI_TYPE_GUI) {
        QMainWindow *mainWindow = new GUI::MainWindow();
        mainWindow->show();
    } else {
        std::list<RunInterface::MultiRunConfig> runConfig;
        if (parser.getConfigInputType() == Application::HeadlessConfigInputType::INPUT_JSON_PATH) {
            auto configList = parser.parseJSONConfig();
            if (std::holds_alternative<QString>(configList)) {
                qCritical() << std::get<QString>(configList) << "\nExiting the application!";
                return 1;
            }
            runConfig = std::get<std::list<RunInterface::MultiRunConfig>>(configList);
        } else {
            auto config = parser.getRunConfig();
            if (std::holds_alternative<QString>(config)) {
                qCritical() << std::get<QString>(config) << "\nExiting the application!";
                return 1;
            }
            runConfig.push_back(std::get<RunInterface::MultiRunConfig>(config));
        }
        headlessSolveAllNoWait(runConfig);
    }
    return QCoreApplication::exec();
}

