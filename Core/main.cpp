#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <memory>

#include "gui_main_window.h"

int main(int argc, char *argv[]) {
    std::unique_ptr<QCoreApplication> pathfindingApp = std::make_unique<QCoreApplication>(argc,argv);
    QCoreApplication::setApplicationName("Pathfinding");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser pathfindingParser;
    pathfindingParser.setApplicationDescription(
            "Command-line and GUI Application for pathfinding algorithm benchmarking");
    pathfindingParser.addHelpOption();
    pathfindingParser.addVersionOption();

    QCommandLineOption guiOption(QStringList() << "g" << "gui", "Starts the application in GUI mode");
    pathfindingParser.addOption(guiOption);

    QCommandLineOption headlessOption(QStringList() << "h" << "headless", "Starts the application in headless mode");
    pathfindingParser.addOption(guiOption);


    pathfindingParser.process(*pathfindingApp);

    bool guiMode = pathfindingParser.isSet(guiOption);
    bool headlessMode = pathfindingParser.isSet(headlessOption);




    if(guiMode){
        pathfindingApp = std::make_unique<QApplication>(argc, argv);
        QMainWindow* mainWindow = new GUI::MainWindow();
        mainWindow->show();
    }else{
        //TODO: implement this case
    }



    return QCoreApplication::exec();
}