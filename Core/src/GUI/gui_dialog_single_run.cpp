#include "GUI/gui_dialog_single_run.h"

#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <algorithm>

GUI::SingleRunDialog::SingleRunDialog(RunInterface::RunGridConfig config,
                                      Pathfinder::PathfinderStrategy strat,
                                      QWidget *parent) :
        QDialog(parent),
        runInterface(new RunInterface::SingleRun(config, strat)),
        singleRunThread(new QThread(this)),
        nextStepButton(new QPushButton("Next Step", this)),
        toggleRunButton(new QPushButton("Toggle", this)),
        gridWidget(new GridDrawerWidget(runInterface->getGridRef(), this)),
        mainLayout(new QVBoxLayout(this)),
        gridWidgetLayout(new QHBoxLayout()),
        buttonLayout(new QHBoxLayout()) {
    showMaximized();
    runInterface->moveToThread(singleRunThread);
    connect(this, SIGNAL(nextStep()), runInterface, SLOT(nextStep()));
    connect(runInterface, SIGNAL(stepFinished()), this, SLOT(onStepFinished()));
    connect(singleRunThread, SIGNAL(finished()), runInterface, SLOT(deleteLater()));
    singleRunThread->start();

    connect(nextStepButton, &QPushButton::clicked, this,
            &SingleRunDialog::nextStepButtonHandler);
    connect(toggleRunButton, &QPushButton::clicked, this,
            &SingleRunDialog::toggleRunButtonHandler);

    buttonLayout->addWidget(toggleRunButton);
    buttonLayout->addWidget(nextStepButton);
    gridWidgetLayout->addWidget(gridWidget);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gridWidgetLayout);

    this->setLayout(mainLayout);
}

void GUI::SingleRunDialog::onStepFinished() {
    gridWidget->update();
}

void GUI::SingleRunDialog::toggleRunButtonHandler() {

}

void GUI::SingleRunDialog::nextStepButtonHandler() {
    emit nextStep();
}
