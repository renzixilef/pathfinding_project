#include "command_line_parser.h"


void Application::PathfindingCommandParser::addOption(const QCommandLineOption &option, const QStringList &setIds) {
    for (const auto &id: setIds) optionSets[id].insert(OptionWrapper(option));
    this->QCommandLineParser::addOption(option);
}

QPair<bool, QString> Application::PathfindingCommandParser::inputOptionsValid() const {
    QSet<QString> sharedSets;
    bool isFirstOption = true;

    for (auto it = optionSets.constBegin(); it != optionSets.constEnd(); it++) {
        QSet<QString> currentOptionSets;
        for (const OptionWrapper &opt: it.value()) {
            if (this->isSet(opt.getOption())) {
                currentOptionSets.insert(it.key());
                if (!isFirstOption && !sharedSets.intersects(currentOptionSets)) {
                    return qMakePair(false, opt.getOption().names().first());
                }
            }
        }
        if (isFirstOption) {
            sharedSets.swap(currentOptionSets);
            isFirstOption = false;
        } else {
            sharedSets.intersect(currentOptionSets);
        }
    }
    return qMakePair(true, QString());
}

Application::PathfindingCommandParser::PathfindingCommandParser()
        : QCommandLineParser(),
          guiOption(QStringList() << "g" << "gui", "Starts the application in GUI mode."),
          headlessOption(QStringList() << "he" << "headless", "Starts the application in headless mode."),
          headlessJSONConfigOption(QStringList() << "j" << "json-path",
                                   "Specify the path to a json file containing the run configuration.",
                                   "json-path"),
          gridDimensionsOption(QStringList() << "gd" << "grid-dimensions",
                               "Specify the grids dimensions as 'width:height'", "dimensions"),
          obstacleGenOption(QStringList() << "oa" << "obstacle-algorithm",
                            "Specify the algorithm to use for obstacle generation.",
                            "obstacle-algorithm"),
          obstacleDensityOption(QStringList() << "od" << "obstacle-density",
                                "Specify the obstacle-density between 0-1 for obstacle generation.",
                                "obstacle-density"),
          minStartEndOption(QStringList() << "se" << "start-end",
                            "Specify the minimum start to end distance, 0-1 (times shorter side len).",
                            "start-end-distance"),
          solverOption(QStringList() << "pa" << "pathfinding-algorithm",
                       "Specify the pathfinding algorithm to use for the generated grid.",
                       "pathfinding-algorithm"){
    setApplicationDescription(
            "Command-line and GUI Application for pathfinding algorithm benchmarking");

    addOption(guiOption, QStringList() << "gui");

    addOption(headlessOption, QStringList() << "headless");

    addOption(headlessJSONConfigOption, QStringList() << "headless" << "multi-run");

    addOption(gridDimensionsOption, QStringList() << "headless" << "single-run");

    addOption(obstacleGenOption, QStringList() << "headless" << "single-run");

    addOption(obstacleDensityOption, QStringList() << "headless" << "single-run");

    addOption(minStartEndOption, QStringList() << "headless" << "single-run");

    addOption(solverOption, QStringList() << "headless" << "single-run");

    addHelpOption();
    addVersionOption();
}
