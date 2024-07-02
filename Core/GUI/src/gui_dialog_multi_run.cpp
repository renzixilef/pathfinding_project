#include "gui_dialog_multi_run.h"

#include <QPushButton>
#include "gui_dialog_evaluation.h"

GUI::MultiRunDialog::MultiRunDialog(std::queue<std::tuple<RunInterface::RunGridConfig,
        std::list<Pathfinder::PathfinderStrategy>, QString>> &queue,
                                    QWidget *parent) :

        QDialog(parent),
        runQueue(queue),
        multiRunThread(new QThread(this)),
        toggleRunButton(new QPushButton("Play")),
        moveToEvaluationButton(new QPushButton("Evaluation")),
        mainLayout(new QVBoxLayout(this)),
        buttonLayout(new QHBoxLayout()),
        runProgressView(new Widgets::RunProgressView()) {

    qRegisterMetaType<Pathfinder::PathfinderPerformanceMetric>("Pathfinder::PathfinderPerformanceMetric");
    qRegisterMetaType<RunInterface::RunGridConfig>("RunInterface::RunGridConfig");
    qRegisterMetaType<std::list<Pathfinder::PathfinderStrategy>>("std::list<Pathfinder::PathfinderStrategy>");
    qRegisterMetaType<int32_t>("int32_t");

    auto nextConfig = runQueue.front();
    setDisplayableStringForCurrentConfig(nextConfig);
    shouldRepeatUnsolvables = std::get<0>(nextConfig).repeatUnsolvables.value();

    runInterface = new RunInterface::MultiRun(std::get<0>(nextConfig), std::get<1>(nextConfig));
    runInterface->moveToThread(multiRunThread);
    setupConnections();
    multiRunThread->start();

    toggleRunButton->setStyleSheet("background-color: green;");

    moveToEvaluationButton->hide();
    moveToEvaluationButton->setStyleSheet("background-color: blue;");
    moveToEvaluationButton->setDisabled(true);

    runProgressView->addNewConfig(std::get<2>(nextConfig));
    runProgressView->updateProgress(std::get<2>(nextConfig), 0);

    buttonLayout->addWidget(toggleRunButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(runProgressView);
    mainLayout->addWidget(moveToEvaluationButton);
    setLayout(mainLayout);

}

void GUI::MultiRunDialog::setupConnections() {
    connect(this, SIGNAL(nextRun()),
            runInterface, SLOT(nextRun()));

    connect(runInterface, SIGNAL(solverFinished(const Pathfinder::PathfinderPerformanceMetric &,
                                         int32_t)),
            this, SLOT(onSolverFinished(const Pathfinder::PathfinderPerformanceMetric&,
                               int32_t)));

    connect(this, SIGNAL(sendNewData(const RunInterface::RunGridConfig &,
                                 const std::list<Pathfinder::PathfinderStrategy> & )),
            runInterface, SLOT(onNewData(const RunInterface::RunGridConfig &,
                                       const std::list<Pathfinder::PathfinderStrategy> & )));

    connect(this, SIGNAL(nextGrid()),
            runInterface, SLOT(createNewGridWithCurrentConfig()));

    connect(multiRunThread, SIGNAL(finished()),
            runInterface, SLOT(deleteLater()));

    connect(toggleRunButton, &QPushButton::clicked,
            this, &MultiRunDialog::toggleRunButtonHandler);

    connect(moveToEvaluationButton, &QPushButton::clicked,
            this, &MultiRunDialog::moveToEvaluationButtonHandler);
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

void GUI::MultiRunDialog::onSolverFinished(const Pathfinder::PathfinderPerformanceMetric &pathfinderExit,
                                           int32_t exitInt) {
    using RunInterface::RunnerReturnStatus;
    auto currentConfig = runQueue.front();
    auto exit = static_cast<RunnerReturnStatus>(exitInt);
    switch (exit) {
        case RunnerReturnStatus::RETURN_UNSOLVABLE:
            incrementUnsolvableCountForConfig(currentConfig);
            if (!shouldRepeatUnsolvables) {
                gridIterator++;
                runProgressView->updateProgress(std::get<2>(currentConfig),
                                                static_cast<int32_t>(gridIterator * 100 /
                                                                     std::get<0>(currentConfig).iterations.value()));
            }
            if (gridIterator < std::get<0>(currentConfig).iterations.value()) {
                emit nextGrid();
            } else {
                handleNewConfigDemand();
            }
            break;
        case RunnerReturnStatus::RETURN_LAST_GRID_DONE:
            pushBackPathfinderExitForCurrentConfig(pathfinderExit, currentConfig);
            gridIterator++;
            runProgressView->updateProgress(std::get<2>(currentConfig), 100);
            handleNewConfigDemand();
            break;
        case RunnerReturnStatus::RETURN_LAST_SOLVER_DONE:
            gridIterator++;
            emit nextGrid();
            [[fallthrough]];
        case RunnerReturnStatus::RETURN_NORMAL:
            pushBackPathfinderExitForCurrentConfig(pathfinderExit, currentConfig);
            runProgressView->updateProgress(std::get<2>(currentConfig),
                                            static_cast<int32_t>(gridIterator * 100 /
                                                                 std::get<0>(currentConfig).iterations.value()));
            break;
    }
    updateGUIAfterFinishedRun();
    if(!finished && !runPaused){
        emit nextRun();
    }
}

void GUI::MultiRunDialog::updateGUIAfterFinishedRun(){
    if (finished) {
        toggleRunButton->setDisabled(true);
        toggleRunButton->setStyleSheet("background-color:gray;");
        toggleRunButton->setText("Play");

        moveToEvaluationButton->show();
        moveToEvaluationButton->setEnabled(true);
    } else if (runPaused) {
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green;");
    }
}

void GUI::MultiRunDialog::handleNewConfigDemand() {
    runQueue.pop();
    if (!runQueue.empty()) {
        auto nextConfig = runQueue.front();
        setDisplayableStringForCurrentConfig(nextConfig);
        shouldRepeatUnsolvables = std::get<0>(nextConfig).repeatUnsolvables.value();
        emit sendNewData(std::get<0>(nextConfig), std::get<1>(nextConfig));
        runProgressView->addNewConfig(std::get<2>(nextConfig));
        runProgressView->updateProgress(std::get<2>(nextConfig), 0);
        gridIterator = 0;
        emit nextGrid();
    } else {
        finished = true;
    }
}

void GUI::MultiRunDialog::moveToEvaluationButtonHandler() {
    auto evalDialog = new EvaluationDialog(evalMap, this);

    evalDialog->exec();
}

void GUI::MultiRunDialog::closeEvent(QCloseEvent *event) {
    runInterface->terminate();
    QThread::msleep(1000);
    multiRunThread->quit();
    if (!multiRunThread->wait(3000)) {
        multiRunThread->terminate();
        multiRunThread->wait();
    }
    QDialog::closeEvent(event);
}
