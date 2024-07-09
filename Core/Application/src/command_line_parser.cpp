#include "command_line_parser.h"

#include "define.h"
#include <QRegularExpression>
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>


void Application::PathfindingCommandParser::addOption(const QCommandLineOption &option, const QStringList &setIds) {
    for (const auto &id: setIds) optionSets[id].insert(OptionWrapper(option));
    this->QCommandLineParser::addOption(option);
}

QPair<bool, QString> Application::PathfindingCommandParser::inputOptionsValid() const {
    QSet<QString> sharedSets;
    bool isFirstOption = false;
    for (auto it = optionSets.constBegin(); it != optionSets.constEnd(); it++) {
        QSet<QString> currentOptionSets;
        for (const OptionWrapper &opt: it.value()) {
            if (this->isSet(opt.getOption())) {
                currentOptionSets.insert(it.key());
                if (isFirstOption && !sharedSets.intersects(currentOptionSets)) {
                    return qMakePair(false, opt.getOption().names().first());
                }
                isFirstOption = true;
            }
        }
        if (isFirstOption)
            sharedSets = sharedSets.isEmpty() ? currentOptionSets : sharedSets.intersect(currentOptionSets);
    }
    return qMakePair(true, QString());
}

Application::PathfindingCommandParser::PathfindingCommandParser()
        : QCommandLineParser(),
          guiOption(QStringList() << "g" << "gui", "Starts the application in GUI mode."),
          headlessOption(QStringList() << "headless", "Starts the application in headless mode."),
          headlessJSONConfigOption(QStringList() << "j" << "json-path",
                                   "Specify the path to a json file containing the run configuration.",
                                   "json-path"),
          gridDimensionsOption(QStringList() << "grid-dimensions",
                               "Specify the grids dimensions as 'width:height'", "dimensions"),
          obstacleGenOption(QStringList() << "obstacle-algorithm",
                            "Specify the algorithm to use for obstacle generation as an uint in [1,4].\n"
                            "1: Random Obstacle Generator\n2: Wall-like Obstacle Generator\n"
                            "3: Drunken-Walk Obstacle Generator\n4: Perlin-Noise Obstacle Generator",
                            "obstacle-algorithm"),
          obstacleDensityOption(QStringList() << "obstacle-density",
                                "Specify the obstacle-density between 0-1 for obstacle generation.",
                                "obstacle-density"),
          minStartEndOption(QStringList() << "start-end",
                            "Specify the minimum start to end distance, 0-1 (times shorter side length).",
                            "start-end-distance"),
          solverOption(QStringList() << "pathfinding-algorithm",
                       "Specify the pathfinding algorithm to use for the generated grid as uint in [1,3].\n"
                       "1: Dijkstra's Algorithm\n2: A* Algorithm\n3: Jump-Point-Search Algorithm",
                       "pathfinding-algorithm"),
          iterationsOption(QStringList() << "i" << "iterations"
                                         << "Specify the number of iterations for the input configuration as uint."
                                         << "iterations") {
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

    addOption(iterationsOption, QStringList() << "headless" << "single-run");

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


std::variant<RunInterface::MultiRunConfig, QString>
Application::PathfindingCommandParser::getRunConfig() const {
    RunInterface::RunGridConfig thisConfig{DEFAULT_GRID_WIDTH,
                                           DEFAULT_GRID_HEIGHT,
                                           DEFAULT_OBSTACLE_DENSITY,
                                           DEFAULT_START_END_DISTANCE};
    std::list<Pathfinder::PathfinderStrategy> solverStrats;
    if (isSet(gridDimensionsOption)) {
        auto gridDims = parseWithRegex(value(gridDimensionsOption),
                                       QRegularExpression(R"(^(\d+):(\d+)$)"));
        if (gridDims == std::nullopt) return "Could not parse Grid dimensions, expected input 'width:height'.";
        thisConfig.gridWidth = gridDims.value()[0].toUInt();
        thisConfig.gridHeight = gridDims.value()[1].toUInt();
    } else
        printDefaultingToMessage(gridDimensionsOption,
                                 QString("Width: %1 Height: %2.").arg(DEFAULT_GRID_WIDTH, DEFAULT_GRID_HEIGHT));

    if (isSet(obstacleGenOption)) {
        auto obstacleGen = parseWithRegex(value(obstacleGenOption),
                                          QRegularExpression(R"(^\b[1-4]\b$)"));
        if (obstacleGen == std::nullopt) return "Could not parse Obstacle Generator, expected uint in [1,4].";
        thisConfig.obstacleGenStrategy = static_cast<GridGenerator::ObstacleGenStrategy>(
                obstacleGen.value()[0].toUInt());
    } else printDefaultingToMessage(gridDimensionsOption, QString("Random Obstacle Generator."));

    if (isSet(obstacleDensityOption)) {
        auto obstacleDensity = parseWithRegex(value(obstacleDensityOption),
                                              QRegularExpression(R"(^0(\.\d)?|0\.[0-6]\d*$|0\.7{1}0*$)"));
        if (obstacleDensity == std::nullopt) return "Could not parse Obstacle Density, expected float in [0,0.7].";
        thisConfig.obstacleDensity = obstacleDensity.value()[0].toFloat();
    } else printDefaultingToMessage(gridDimensionsOption, QString("%1").arg(DEFAULT_OBSTACLE_DENSITY));

    if (isSet(minStartEndOption)) {
        auto minStartEndDistance = parseWithRegex(value(minStartEndOption),
                                                  QRegularExpression(R"(^(0(\.\d*)?|0.\d*[1-9]\d*|1(\.0*)?)$)"));
        if (minStartEndDistance == std::nullopt) return "Could not parse min Distance, expected float in [0,1]";
        thisConfig.minStartEndDistance = minStartEndDistance.value()[0].toFloat();
    } else printDefaultingToMessage(gridDimensionsOption, QString("%1").arg(DEFAULT_START_END_DISTANCE));

    if (isSet(solverOption)) {
        auto solver = parseWithRegex(value(solverOption),
                                     QRegularExpression(R"(^\[\s?(?:(1|2|3)\s?,\s?){0,2}(1|2|3)?\s?\]$)"));
        if (solver == std::nullopt) return "Could not parse Solver, expected list of unique uints in [1,3].";
        auto uniqueStrats = QSet<QString>(solver.value().begin(), solver.value().end());
        for (const QString &strat: uniqueStrats) {
            solverStrats.push_back(static_cast<Pathfinder::PathfinderStrategy>(strat.toUInt()));
        }
    } else {
        auto strat = static_cast<Pathfinder::PathfinderStrategy>(DEFAULT_SOLVER);
        const std::string
                &stratText = Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.at(strat);
        printDefaultingToMessage(gridDimensionsOption, QString::fromStdString(stratText));
        solverStrats.push_back(strat);
    }

    if (isSet(iterationsOption)) {
        auto iterations = parseWithRegex(value(iterationsOption),
                                         QRegularExpression(R"(^[1-9]\\d*$)"));
        if (iterations == std::nullopt) return "Could not parse iterations, expected uint";
        thisConfig.iterations = iterations.value()[0].toUInt();
    } else {
        thisConfig.iterations = DEFAULT_ITERATIONS;
        printDefaultingToMessage(gridDimensionsOption, QString("%1").arg(1));
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

std::variant<std::list<RunInterface::MultiRunConfig>, QString>
Application::PathfindingCommandParser::parseJSONConfig() {
    QString filename = value(headlessJSONConfigOption);
    QFile jsonFile(filename);
    if (!jsonFile.open(QFile::ReadOnly)) return QString("Failed to open JSON file: %1").arg(filename);
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonFile.readAll()));
    if (jsonDoc.isNull() || !jsonDoc.isObject()) return QString("Failed to read from JSON file: %1").arg(filename);

    QJsonValue jsonValue = jsonDoc.object();
    std::list<RunInterface::MultiRunConfig> configList;
    QJsonArray json;
    if (jsonValue.isObject()) json.append(jsonValue.toObject());
    else if (jsonValue.isArray()) json = jsonValue.toArray();
    else return QString("Failed to parse JSON file: Invalid JSON input!");

    for (const QJsonValueRef &value: json) {
        if (!value.isObject()) return QString("Failed to parse JSON file: Invalid JSON input!");
        QJsonObject object = value.toObject();
        if (!object.contains("gridConfig") || !object["gridConfig"].isObject())
            return QString("Failed to parse JSON file: gridConfig not found or is not an object");
        if (!object.contains("strats") || !object["strats"].isArray())
            return QString("Failed to parse JSON file: strats not found or is not an array");
        QJsonObject gridConfigJson = object["gridConfig"].toObject();
        QJsonArray stratsJson = object["strats"].toArray();

        auto gridConfig = parseJSONGridConfig(gridConfigJson);
        if (std::holds_alternative<QString>(gridConfig)) return std::get<QString>(gridConfig);
        auto strats = parseJSONStrats(stratsJson);
        if (std::holds_alternative<QString>(strats)) return std::get<QString>(strats);
        configList.push_back({std::get<RunInterface::RunGridConfig>(gridConfig),
                              std::get<std::list<Pathfinder::PathfinderStrategy>>(strats)});
    }
    return configList;
}

std::variant<RunInterface::RunGridConfig, QString>
Application::PathfindingCommandParser::parseJSONGridConfig(const QJsonObject &gridConfigJson) {
    RunInterface::RunGridConfig gridConfig{};
    if (gridConfigJson.contains("gridWidth")) {
        QJsonValue gridWidth = gridConfigJson["gridWidth"];
        if (!gridWidth.isDouble() || gridWidth.toInt() < 0)
            return QString("Failed to parse JSON file: gridWidth must be a positive integer.");
        gridConfig.gridWidth = gridWidth.toInt();
    } else return QString("Failed to parse JSON file: Field gridWidth not present!");


    if (gridConfigJson.contains("gridHeight")) {
        QJsonValue gridHeight = gridConfigJson["gridHeight"];
        if (!gridHeight.isDouble() || gridHeight.toInt() < 0)
            return QString("Failed to parse JSON file: gridHeight  must be a positive integer.");
        gridConfig.gridHeight = gridHeight.toInt();
    } else return QString("Failed to parse JSON file: Field gridHeight not present!");

    if (gridConfigJson.contains("obstacleDensity")) {
        QJsonValue obstacleDensity = gridConfigJson["obstacleDensity"];
        if (!obstacleDensity.isDouble() || obstacleDensity.toDouble() < 0.0 || obstacleDensity.toDouble() > 0.7)
            return QString("Failed to parse JSON file: obstacleDensity must be a float between 0.0 and 0.7.");
        gridConfig.obstacleDensity = static_cast<float>(obstacleDensity.toDouble());
    } else return QString("Failed to parse JSON file: Field obstacleDensity not present!");

    if (gridConfigJson.contains("obstacleGenStrategy")) {
        QJsonValue obstacleGenStrategy = gridConfigJson["obstacleGenStrategy"];
        if (!obstacleGenStrategy.isDouble() || obstacleGenStrategy.toInt() < 1 || obstacleGenStrategy.toInt() > 4)
            return QString("Failed to parse JSON file: obstacleGenStrategy must be a uint between 1-4.");
        gridConfig.obstacleGenStrategy =
                static_cast<GridGenerator::ObstacleGenStrategy>(obstacleGenStrategy.toInt());
    } else return QString("Failed to parse JSON file: Field obstacleGenStrategy not present!");

    if (gridConfigJson.contains("iterations")) {
        QJsonValue iterations = gridConfigJson["iterations"];
        if (!iterations.isDouble() || iterations.toInt() <= 0)
            return QString("'iterations' should be a positive integer.");
        gridConfig.iterations = iterations.toInt();
    } else return QString("Failed to parse JSON file: Field iterations not present");

    if (gridConfigJson.contains("minStartEndDistance")) {
        QJsonValue startEndDist = gridConfigJson["minStartEndDistance"];
        if (!startEndDist.isDouble() || startEndDist.toDouble() < 0.0 || startEndDist.toDouble() > 1.0)
            return QString("'minStartEndDistance' must be a float between 0.0 and 1.0.");
        gridConfig.minStartEndDistance = static_cast<float>(startEndDist.toDouble());
    } else  return QString("Failed to parse JSON file: Field minStartEndDistance not present");

    if (gridConfigJson.contains("repeatUnsolvables")) {
        QJsonValue repeatUnsolvables = gridConfigJson["repeatUnsolvables"];
        if (!repeatUnsolvables.isBool())
            return QString("Failed to parse JSON file: repeatUnsolvables must be a boolean.");
        gridConfig.repeatUnsolvables = repeatUnsolvables.toBool();
    } else return QString("Failed to parse JSON file: Field repeatUnsolvables not present!");

    return gridConfig;
}

std::variant<std::list<Pathfinder::PathfinderStrategy>, QString>
Application::PathfindingCommandParser::parseJSONStrats(const QJsonArray &stratsJson) {
    std::list<Pathfinder::PathfinderStrategy> strats;
    for (const QJsonValue &strat: stratsJson) {
        if (!strat.isDouble())
            return QString("strat value should be an uint in [1,3].");

        int stratInt = strat.toInt();
        if (stratInt < 1 || stratInt > 3)
            return QString("strat value should be between 1 and 3.");
        strats.push_back(static_cast<Pathfinder::PathfinderStrategy>(stratInt));
    }
    return strats;
}




