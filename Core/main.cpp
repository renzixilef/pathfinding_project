#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "command_line_parser.h"
#include "gui_main_window.h"
#include "headless_multi_run.h"

// TODO: repeatUnsolvables, error message if over MAX_TRIES maybe?

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
        return QCoreApplication::exec();
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
        std::queue<std::pair<RunInterface::MultiRunConfig, QString>> runQueue;
        for (const auto &singleConfig: runConfig) {
            QString itemText = Application::HeadlessRunner::generateConfigQString(singleConfig);
            runQueue.emplace(singleConfig, itemText);
        }
        Application::HeadlessRunner headlessRunner(runQueue);
        headlessRunner.headlessSolveAllNoWait();
        return 0;
    }
}

