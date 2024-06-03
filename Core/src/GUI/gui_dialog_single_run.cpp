#include "GUI/gui_dialog_single_run.h"

#include <QThread>

GUI::SingleRunDialog::SingleRunDialog(RunInterface::RunGridConfig config,
                                      Pathfinder::PathfinderStrategy strat,
                                      QWidget *parent) :
        QDialog(parent),
        runInterface(new RunInterface::SingleRun(config, strat)),
        nextStepButton(new QPushButton(this)),
        toggleRunButton(new QPushButton(this)),
        singleRunThread(new QThread(this)) {

    runInterface->moveToThread(singleRunThread);
    connect(this, SIGNAL(nextStep()), runInterface, SLOT(nextStep()));
    connect(runInterface, SIGNAL(stepFinished()), this, SLOT(onStepFinished()));
    connect(singleRunThread, SIGNAL(finished()), runInterface, SLOT(deleteLater()));
    singleRunThread->start();

    connect(nextStepButton, &QPushButton::clicked, this,
            &SingleRunDialog::nextStepButtonHandler);
    connect(toggleRunButton, &QPushButton::clicked, this,
            &SingleRunDialog::toggleRunButtonHandler);


    //runInterface.start();
}

void GUI::SingleRunDialog::onStepFinished() {

}

void GUI::SingleRunDialog::toggleRunButtonHandler() {

}

void GUI::SingleRunDialog::nextStepButtonHandler() {

}
