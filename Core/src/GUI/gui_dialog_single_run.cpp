#include "GUI/gui_dialog_single_run.h"

#include <QThread>

GUI::SingleRunDialog::SingleRunDialog(RunInterface::RunGridConfig config,
                                      Pathfinder::PathfinderStrategy strat,
                                      QWidget *parent) :
        runInterface(new RunInterface::SingleRun(config, strat)),
        nextStepButton(new QPushButton(this)),
        startRunNoWaitButton(new QPushButton(this)),
        singleRunThread(new QThread(this)){

    runInterface->moveToThread(singleRunThread);
    connect(this, SIGNAL(nextStep()), runInterface, SLOT(nextStep()));
    connect(runInterface, SIGNAL(stepFinished()), this, SLOT(onStepFinished()));
    connect(singleRunThread,SIGNAL(finished()), runInterface, SLOT(deleteLater()));
    singleRunThread->start();



    //runInterface.start();
}
