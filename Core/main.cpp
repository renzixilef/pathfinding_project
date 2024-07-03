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

    auto& parser = Application::PathfindingCommandParser::getInstance();
    // TODO: maybe move options to pathfindingParser constructor
    // TODO: better description for command line options


    parser.process(pathfindingApp);
    auto parserInputValid = parser.inputOptionsValid();

    if(!parserInputValid.first){
        qCritical() << "Incompatible flag set: " << parserInputValid.second <<"\nExiting the programm!";
        QApplication::exit(1);
    }


    if (guiMode) {
        QMainWindow *mainWindow = new GUI::MainWindow();
        mainWindow->show();
    } else {
        //TODO: implement this case
    }
    return QCoreApplication::exec();
}