#include "GUI/gui_dialog_multi_run.h"

#include <QPushButton>

GUI::MultiRunDialog::MultiRunDialog(std::queue<std::pair<RunInterface::RunGridConfig,
        std::list<Pathfinder::PathfinderStrategy>>> queue,
                                    QWidget *parent) :

        QDialog(parent),
        runQueue(std::move(queue)),
        multiRunThread(new QThread(this)),
        toggleRunButton(new QPushButton("Play", this)),
        mainLayout(new QVBoxLayout(this)),
        buttonLayout(new QHBoxLayout()) {
    toggleRunButton->setStyleSheet("background-color: green;");

    auto nextConfig = runQueue.front();

    runInterface = new RunInterface::MultiRun(nextConfig.first, nextConfig.second);
    runInterface->moveToThread(multiRunThread);

    setupConnections();

    multiRunThread->start();

    buttonLayout->addWidget(toggleRunButton);
    mainLayout->addLayout(buttonLayout);
    //mainLayout->addWidget(RunView);
    setLayout(mainLayout);

}

void GUI::MultiRunDialog::setupConnections() {
    connect(this, SIGNAL(nextRun()),
            runInterface, SLOT(nextRun()));

    connect(runInterface, SIGNAL(solverFinished()),
            this, SLOT(onSolverFinished()));

    connect(runInterface, SIGNAL(gridFinished()),
            this, SLOT(onGridFinished()));

    connect(runInterface, SIGNAL(demandNewConfiguration()),
            this, SLOT(onNewConfigDemand()));

    connect(this, SIGNAL(sendNewData()),
            runInterface, SLOT(onNewData()));

    connect(multiRunThread, SIGNAL(finished()),
            runInterface, SLOT(deleteLater()));

    connect(toggleRunButton, &QPushButton::clicked,
            this, &MultiRunDialog::toggleRunButtonHandler);
}

void GUI::MultiRunDialog::toggleRunButtonHandler() {
    if (runPaused) {
        toggleRunButton->setText("Pause");
        toggleRunButton->setStyleSheet("background-color: red");
        runPaused = false;
        emit nextRun();
    } else {
        runPaused = true;
    }
}

void GUI::MultiRunDialog::onGridFinished() {
    //TODO: evaluate solved grids
    //TODO: emit signal to reconstruct solvers
}

void GUI::MultiRunDialog::onSolverFinished() {
    if(runPaused){
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green");
    }else{
        emit nextRun();
    }
}

void GUI::MultiRunDialog::onNewConfigDemand() {
    onGridFinished();
    runQueue.pop();
    if(!runQueue.empty()){
        auto nextConfig = runQueue.front();
        emit sendNewData(nextConfig.first, nextConfig.second);
        if(runPaused){
            toggleRunButton->setText("Play");
            toggleRunButton->setStyleSheet("background-color: green");
        }else{
            emit nextRun();
        }
    }
    //TODO: update runView widget
}
