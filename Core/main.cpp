#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "command_line_parser.h"
#include "gui_main_window.h"

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
        std::variant<std::list<RunInterface::MultiRunConfig>, RunInterface::MultiRunConfig> runConfig;
        if (parser.getConfigInputType() == Application::HeadlessConfigInputType::INPUT_JSON_PATH) {
            auto configList = parser.parseJSONConfig();
            if (std::holds_alternative<QString>(configList)) {
                qCritical() << std::get<QString>(configList) << "\nExiting the application!";
                return 1;
            }
            runConfig = std::get<std::list<RunInterface::MultiRunConfig>>(configList);
        } else {
            auto config = parser.getRunConfig();
            if (std::holds_alternative<QString>(config.value())) {
                qCritical() << std::get<QString>(config.value()) << "\nExiting the application!";
                return 1;
            }
            runConfig = std::get<RunInterface::MultiRunConfig>(config.value());
        }
        std::visit([](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, RunInterface::MultiRunConfig>) {


            } else if constexpr (std::is_same_v<T, std::list<RunInterface::MultiRunConfig>>) {

            }
        }, runConfig);



        //TODO: handle headless multi runner creation etc.
    }
    return QCoreApplication::exec();
}