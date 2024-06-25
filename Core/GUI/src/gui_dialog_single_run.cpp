#include "gui_dialog_single_run.h"

#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>

GUI::SingleRunDialog::SingleRunDialog(const RunInterface::RunGridConfig &config,
                                      const Pathfinder::PathfinderStrategy &strat,
                                      QWidget *parent) :
        QDialog(parent),
        runInterface(new RunInterface::SingleRun(config, strat)),
        singleRunThread(new QThread(this)),
        nextStepButton(new QPushButton("Next Step")),
        toggleRunButton(new QPushButton("Play")),
        serializeRunForDebugButton(new QPushButton("Debug Save")),
        toggleStartEndRedefinitionButton(new QPushButton("Choose Start/End")),
        gridWidget(new Widgets::GridDrawerWidget(runInterface->getGridRef())),
        mainLayout(new QVBoxLayout(this)),
        gridWidgetLayout(new QHBoxLayout()),
        buttonLayout(new QHBoxLayout()),
        nextStepTimer(new QTimer(this)) {

    qRegisterMetaType<std::string>("std::string");

    toggleRunButton->setStyleSheet("background-color: green");
    toggleRunButton->setStyleSheet("color:white");

    serializeRunForDebugButton->hide();
    showMaximized();
    runInterface->moveToThread(singleRunThread);

    setupConnections();

    singleRunThread->start();

    buttonLayout->addWidget(toggleRunButton);
    buttonLayout->addWidget(nextStepButton);
    buttonLayout->addWidget(toggleStartEndRedefinitionButton);
    buttonLayout->addWidget(serializeRunForDebugButton);
    gridWidgetLayout->addWidget(gridWidget);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gridWidgetLayout);

    setLayout(mainLayout);
}

void GUI::SingleRunDialog::onStepFinished() {
    gridWidget->update();
    if (!runPaused) {
        nextStepTimer->setSingleShot(true);
        nextStepTimer->start(50);
    } else {
        nextStepButton->setEnabled(true);
        toggleRunButton->setEnabled(true);
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green");
    }
}

void GUI::SingleRunDialog::toggleRunButtonHandler() {
    if (runFinished) {
        runPaused = true;
        runFinished = false;
        serializeRunForDebugButton->hide();
        toggleStartEndRedefinitionButton->setEnabled(true);
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green");
        toggleRunButton->setEnabled(false);
        emit resetRun();
    } else {
        if (runPaused) {
            toggleStartEndRedefinitionButton->setDisabled(true);
            nextStepButton->setEnabled(false);
            toggleRunButton->setText("Pause");
            toggleRunButton->setStyleSheet("background-color: red");
            runPaused = false;
            emit nextStep();
        } else {
            toggleStartEndRedefinitionButton->setDisabled(true);
            runPaused = true;
        }
    }
}

void GUI::SingleRunDialog::nextStepButtonHandler() {
    nextStepButton->setEnabled(false);
    toggleRunButton->setEnabled(false);
    toggleStartEndRedefinitionButton->setDisabled(true);
    emit nextStep();
}

void GUI::SingleRunDialog::onGridFinished() {
    gridWidget->update();
    nextStepButton->setEnabled(false);
    toggleRunButton->setEnabled(true);
    toggleRunButton->setText("New Run");
    toggleRunButton->setStyleSheet("background-color: blue");
    serializeRunForDebugButton->show();
    runFinished = true;
}

void GUI::SingleRunDialog::setupConnections() {
    connect(this, SIGNAL(nextStep()),
            runInterface, SLOT(nextStep()));
    connect(this, SIGNAL(resetRun()),
            runInterface, SLOT(onRunReset()));
    connect(runInterface, SIGNAL(stepFinished()),
            this, SLOT(onStepFinished()));
    connect(runInterface, SIGNAL(gridFinished()),
            this, SLOT(onGridFinished()));
    connect(singleRunThread, SIGNAL(finished()),
            runInterface, SLOT(deleteLater()));
    connect(nextStepButton, &QPushButton::clicked,
            this, &SingleRunDialog::nextStepButtonHandler);
    connect(toggleRunButton, &QPushButton::clicked,
            this, &SingleRunDialog::toggleRunButtonHandler);
    connect(serializeRunForDebugButton, &QPushButton::clicked,
            this, &SingleRunDialog::serializeButtonHandler);
    connect(toggleStartEndRedefinitionButton, &QPushButton::clicked,
            this, &SingleRunDialog::toggleStartEndRedefinitionButtonHandler);
    connect(this, SIGNAL(serialize(const std::string&)),
            runInterface, SLOT(onSerializeRequest(const std::string&)));
    connect(runInterface, SIGNAL(saveDone()),
            this, SLOT(onSaveDone()));
    connect(nextStepTimer, &QTimer::timeout,
            this, &SingleRunDialog::nextStep);

}

void GUI::SingleRunDialog::serializeButtonHandler() {

    QString filename = QFileDialog::getSaveFileName(this, tr("Save Grid As"),
                                                    "/home", tr("Binary Files (*.bin);; All Files(*)"));
    std::string filenameStd = filename.toStdString();

    if(!filenameStd.empty()){
        emit serialize(filenameStd);
        toggleRunButton->setDisabled(true);
    }

}

void GUI::SingleRunDialog::onSaveDone() {
    serializeRunForDebugButton->setEnabled(false);
    serializeRunForDebugButton->setStyleSheet("background-color:green;");
    toggleRunButton->setEnabled(true);
}

void GUI::SingleRunDialog::toggleStartEndRedefinitionButtonHandler() {
    if(gridWidget->getRedefinitionStatus()){
        gridWidget->toggleStartEndRedefinitionPhase();
        toggleRunButton->setEnabled(true);
        nextStepButton->setEnabled(true);
        if(!serializeRunForDebugButton->isHidden()){
            serializeRunForDebugButton->setEnabled(true);
        }
    }else{
        gridWidget->toggleStartEndRedefinitionPhase();
        toggleRunButton->setDisabled(true);
        nextStepButton->setDisabled(true);
        if(!serializeRunForDebugButton->isHidden()){
            serializeRunForDebugButton->setDisabled(true);
        }
        QMessageBox redefinitionExplainerBox;
        redefinitionExplainerBox.setText("How to redefine Start/End");
        redefinitionExplainerBox.setInformativeText("Set start position: Left Click on the new node\n"
                                                    "Set end position: Right Click on the new node");
        redefinitionExplainerBox.exec();
    }
}
