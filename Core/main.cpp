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

    auto &parser = Application::PathfindingCommandParser::getInstance();


    parser.process(pathfindingApp);
    auto parserInputValid = parser.inputOptionsValid();

    if (!parserInputValid.first) {
        qCritical() << "Incompatible flag set: " << parserInputValid.second << "\nExiting the application!";
        QApplication::exit(1);
    }


    if (parser.getUIType() == Application::UIType::UI_TYPE_GUI) {
        QMainWindow *mainWindow = new GUI::MainWindow();
        mainWindow->show();
    } else {
        if (parser.getConfigInputType() == Application::HeadlessConfigInputType::INPUT_JSON_PATH) {
            // TODO: handle json path reading here
        } else {
            auto runConfig = parser.getRunConfig();
            if (std::holds_alternative<QString>(runConfig.value())) {
                qCritical() << std::get<QString>(runConfig.value()) << "\nExiting the application!";
                QApplication::exit(1);
            } else {
                //TODO: handle headless multi runner creation etc.
            }
        }
    }
    return QCoreApplication::exec();
}