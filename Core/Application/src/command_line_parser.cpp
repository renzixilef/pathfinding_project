#include "command_line_parser.h"

#include "define.h"
#include <QRegularExpression>


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

// TODO: better description for command line options
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
                       "pathfinding-algorithm") {
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

Application::UIType Application::PathfindingCommandParser::getUIType() const {
    if (isSet(guiOption)) return Application::UIType::UI_TYPE_GUI;
    else return Application::UIType::UI_TYPE_HEADLESS;
}

std::optional<Application::HeadlessConfigInputType> Application::PathfindingCommandParser::getConfigInputType() const {
    if (isSet(guiOption)) return std::nullopt;
    if (isSet(headlessJSONConfigOption)) return HeadlessConfigInputType::INPUT_JSON_PATH;
    else return HeadlessConfigInputType::INPUT_COMMAND_LINE_FLAGS;

}


std::optional<std::variant<RunInterface::MultiRunConfig, QString>>
Application::PathfindingCommandParser::getRunConfig() const {
    if (isSet(guiOption) || isSet(headlessJSONConfigOption)) return std::nullopt;
    RunInterface::RunGridConfig thisConfig{DEFAULT_GRID_WIDTH,
                                           DEFAULT_GRID_HEIGHT,
                                           DEFAULT_OBSTACLE_DENSITY,
                                           DEFAULT_START_END_DISTANCE};
    std::list<Pathfinder::PathfinderStrategy> solverStrats;
    if (isSet(gridDimensionsOption)) {
        auto gridDims = parseWithRegex(value(gridDimensionsOption),
                                       QRegularExpression(R"((\d+):(\d+))"));
        if (gridDims == std::nullopt) return "Could not parse Grid dimensions, expected input 'width:height'.";
        thisConfig.gridWidth = gridDims.value()[0].toUInt();
        thisConfig.gridHeight = gridDims.value()[1].toUInt();
    }
    if (isSet(obstacleGenOption)) {
        auto obstacleGen = parseWithRegex(value(obstacleGenOption),
                                          QRegularExpression(R"(\b[1-4]\b)"));
        if (obstacleGen == std::nullopt) return "Could not parse Obstacle Generator, expected uint in [1,4].";
        thisConfig.obstacleGenStrategy = static_cast<GridGenerator::ObstacleGenStrategy>(
                obstacleGen.value()[0].toUInt());
    }
    if (isSet(obstacleDensityOption)) {
        auto obstacleDensity = parseWithRegex(value(obstacleDensityOption),
                                              QRegularExpression(R"(^0(\.\d)?|0\.[0-6]\d*$|0\.7{1}0*$)"));
        if (obstacleDensity == std::nullopt) return "Could not parse Obstacle Density, expected float in [0,0.7].";
        thisConfig.obstacleDensity = obstacleDensity.value()[0].toFloat();
    }
    if (isSet(minStartEndOption)) {
        auto minStartEndDistance = parseWithRegex(value(minStartEndOption),
                                                  QRegularExpression(R"(^(0(\.\d*)?|0.\d*[1-9]\d*|1(\.0*)?)$)"));
        if (minStartEndDistance == std::nullopt) return "Could not parse min Distance, expected float in [0,1]";
        thisConfig.minStartEndDistance = minStartEndDistance.value()[0].toFloat();
    }
    if (isSet(solverOption)) {
        auto solver = parseWithRegex(value(solverOption),
                                     QRegularExpression(R"(^\[\s?(?:(1|2|3)\s?,\s?){0,2}(1|2|3)?\s?\]$)"));
        if (solver == std::nullopt) return "Could not parse Solver, expected list of unique uint in [1,3].";
        auto uniqueStrats = QSet<QString>(solver.value().begin(), solver.value().end());
        for (const QString &strat: uniqueStrats) {
            solverStrats.push_back(static_cast<Pathfinder::PathfinderStrategy>(strat.toUInt()));
        }
    } else {
        solverStrats.push_back(static_cast<Pathfinder::PathfinderStrategy>(DEFAULT_SOLVER));
    }
    return RunInterface::MultiRunConfig(thisConfig, solverStrats);
}

std::optional<QStringList>
Application::PathfindingCommandParser::parseWithRegex(const QString &str, QRegularExpression &&regex) {
    QStringList matches;
    QRegularExpressionMatch match = regex.match(str);
    if (!match.hasMatch()) {
        return std::nullopt;
    }
    for (int32_t i = 1; i <= match.lastCapturedIndex(); i++) {
        matches << match.captured(i);
    }
    return matches;
}
