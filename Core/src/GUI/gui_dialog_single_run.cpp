#include "GUI/gui_dialog_single_run.h"

#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <algorithm>

GUI::SingleRunDialog::SingleRunDialog(const RunInterface::RunGridConfig& config,
                                      const Pathfinder::PathfinderStrategy& strat,
                                      QWidget *parent) :
        QDialog(parent),
        runInterface(new RunInterface::SingleRun(config, strat)),
        singleRunThread(new QThread(this)),
        nextStepButton(new QPushButton("Next Step", this)),
        toggleRunButton(new QPushButton("Play", this)),
        gridWidget(new GridDrawerWidget(runInterface->getGridRef(), this)),
        mainLayout(new QVBoxLayout(this)),
        gridWidgetLayout(new QHBoxLayout()),
        buttonLayout(new QHBoxLayout()),
        nextStepTimer(new QTimer(this)){
    toggleRunButton->setStyleSheet("background-color: green");
    toggleRunButton->setStyleSheet("color:white");
    showMaximized();
    runInterface->moveToThread(singleRunThread);
    connect(this, SIGNAL(nextStep()), runInterface, SLOT(nextStep()));
    connect(this, SIGNAL(resetRun()), runInterface, SLOT(onRunReset()));
    connect(runInterface, SIGNAL(stepFinished()), this, SLOT(onStepFinished()));
    connect(runInterface, SIGNAL(gridFinished()), this, SLOT(onGridFinished()));
    connect(singleRunThread, SIGNAL(finished()), runInterface, SLOT(deleteLater()));

    singleRunThread->start();

    connect(nextStepButton, &QPushButton::clicked, this,
            &SingleRunDialog::nextStepButtonHandler);
    connect(toggleRunButton, &QPushButton::clicked, this,
            &SingleRunDialog::toggleRunButtonHandler);
    connect(nextStepTimer, &QTimer::timeout, this, &SingleRunDialog::nextStep);
    buttonLayout->addWidget(toggleRunButton);
    buttonLayout->addWidget(nextStepButton);
    gridWidgetLayout->addWidget(gridWidget);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gridWidgetLayout);

    this->setLayout(mainLayout);
}

void GUI::SingleRunDialog::onStepFinished() {
    gridWidget->update();
    if(!runPaused){
        nextStepTimer->setSingleShot(true);
        nextStepTimer->start(50);
    }else{
        nextStepButton->setEnabled(true);
        toggleRunButton->setEnabled(true);
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green");
    }
}

void GUI::SingleRunDialog::toggleRunButtonHandler() {
    if(runFinished){
        runPaused = true;
        runFinished = false;
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green");
        toggleRunButton->setEnabled(false);
        emit resetRun();
    }else {
        if (runPaused) {
            nextStepButton->setEnabled(false);
            toggleRunButton->setText("Pause");
            toggleRunButton->setStyleSheet("background-color: red");
            runPaused = false;
            emit nextStep();
        } else {
            runPaused = true;
        }
    }
}

void GUI::SingleRunDialog::nextStepButtonHandler() {
    nextStepButton->setEnabled(false);
    toggleRunButton->setEnabled(false);
    emit nextStep();
}

void GUI::SingleRunDialog::onGridFinished() {
    gridWidget->update();
    nextStepButton->setEnabled(false);
    toggleRunButton->setEnabled(true);
    toggleRunButton->setText("New Run");
    toggleRunButton->setStyleSheet("background-color: blue");
    runFinished = true;
}
