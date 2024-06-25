#include <iostream>

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "gui_main_window.h"

int main(int argc, char *argv[]) {
    QApplication pathfindingApp(argc, argv);

    QCommandLineParser pathfindingParser;
    pathfindingParser.setApplicationDescription(
            "Command-line and GUI Application for pathfinding algorithm benchmarking");

    QCommandLineOption guiOption(QStringList() << "g" << "gui", "Starts the application in GUI mode");
    pathfindingParser.addOption(guiOption);

    //TODO: implement additional benchmarking command line options
    //TODO: implement gui and other options mutually exclusive

    pathfindingParser.process(pathfindingApp);
    bool guiMode = pathfindingParser.isSet(guiOption);
    if(guiMode){
        QMainWindow* mainWindow = new GUI::MainWindow();
        mainWindow->show();
    }else{
        //TODO: implement this case
    }



    return QCoreApplication::exec();
}