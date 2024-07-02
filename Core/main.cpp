#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <memory>

#include "command_line_parser.h"
#include "gui_main_window.h"

int main(int argc, char *argv[]) {
    QApplication pathfindingApp(argc, argv);
    QApplication::setApplicationName("Pathfinding");
    QApplication::setApplicationVersion("1.0");

    Application::PathfindingCommandParser pathfindingParser;
    // TODO: maybe move options to pathfindingParser constructor
    // TODO: better description for command line options
    pathfindingParser.setApplicationDescription(
            "Command-line and GUI Application for pathfinding algorithm benchmarking");
    pathfindingParser.addHelpOption();
    pathfindingParser.addVersionOption();

    QCommandLineOption guiOption(QStringList() << "g" << "gui", "Starts the application in GUI mode.");
    pathfindingParser.addOption(guiOption, QStringList() << "gui");

    QCommandLineOption headlessOption(QStringList() << "he" << "headless", "Starts the application in headless mode.");
    pathfindingParser.addOption(headlessOption, QStringList() << "headless");

    QCommandLineOption headlessJSONConfigOption(QStringList() << "j" << "json-path",
                                                "Specify the path to a json file containing the run configuration.",
                                                "json-path");
    pathfindingParser.addOption(headlessJSONConfigOption, QStringList() << "headless" << "multi-run");

    QCommandLineOption gridDimensionsOption(QStringList() << "gd" << "grid-dimensions",
                                            "Specify the grids dimensions as 'width:height'", "dimensions");
    pathfindingParser.addOption(gridDimensionsOption, QStringList() << "headless" << "single-run");

    QCommandLineOption obstacleGenOption(QStringList() << "oa" << "obstacle-algorithm",
                                         "Specify the algorithm to use for obstacle generation.", "obstacle-algorithm");
    pathfindingParser.addOption(obstacleGenOption, QStringList() << "headless" << "single-run");

    QCommandLineOption obstacleDensityOption(QStringList() << "od" << "obstacle-density",
                                             "Specify the obstacle-density between 0-1 for obstacle generation.",
                                             "obstacle-density");
    pathfindingParser.addOption(obstacleDensityOption, QStringList() << "headless" << "single-run");

    QCommandLineOption minStartEndOption(QStringList() << "se" << "start-end",
                                         "Specify the minimum start to end distance, 0-1 (times shorter side len).",
                                         "start-end-distance");
    pathfindingParser.addOption(minStartEndOption, QStringList() << "headless" << "single-run");

    QCommandLineOption solverOption(QStringList() << "pa" << "pathfinding-algorithm",
                                         "Specify the pathfinding algorithm to use for the generated grid.",
                                         "pathfinding-algorithm");
    pathfindingParser.addOption(solverOption, QStringList() << "headless" << "single-run");

    pathfindingParser.process(pathfindingApp);

    bool guiMode = pathfindingParser.isSet(guiOption);
    bool headlessMode = pathfindingParser.isSet(headlessOption);

    if (guiMode) {
        QMainWindow *mainWindow = new GUI::MainWindow();
        mainWindow->show();
    } else {
        //TODO: implement this case
    }
    return QCoreApplication::exec();
}