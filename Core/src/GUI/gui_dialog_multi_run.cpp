#include "GUI/gui_dialog_multi_run.h"

#include <QPushButton>

GUI::MultiRunDialog::MultiRunDialog(std::queue<std::tuple<RunInterface::RunGridConfig,
        std::list<Pathfinder::PathfinderStrategy>, QString>> &queue,
                                    QWidget *parent) :

        QDialog(parent),
        runQueue(queue),
        multiRunThread(new QThread(this)),
        toggleRunButton(new QPushButton("Play")),
        mainLayout(new QVBoxLayout(this)),
        buttonLayout(new QHBoxLayout()),
        runProgressView(new Widgets::RunProgressView()) {
    toggleRunButton->setStyleSheet("background-color: green;");
    qRegisterMetaType<Pathfinder::PathfinderPerformanceMetric>("Pathfinder::PathfinderPerformanceMetric");
    qRegisterMetaType<RunInterface::RunGridConfig>("RunInterface::RunGridConfig");
    qRegisterMetaType<std::list<Pathfinder::PathfinderStrategy>>("std::list<Pathfinder::PathfinderStrategy>");
    auto nextConfig = runQueue.front();

    runInterface = new RunInterface::MultiRun(std::get<0>(nextConfig),
                                              std::get<1>(nextConfig),
                                              shouldRepeatUnsolvables);
    //TODO: Implement shouldRepeatUnsolvables
    runInterface->moveToThread(multiRunThread);

    setupConnections();

    multiRunThread->start();

    runProgressView->addNewConfig(std::get<2>(nextConfig));
    runProgressView->updateProgress(std::get<2>(nextConfig), 0);

    buttonLayout->addWidget(toggleRunButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(runProgressView);
    setLayout(mainLayout);

}

void GUI::MultiRunDialog::setupConnections() {
    connect(this, SIGNAL(nextRun()),
            runInterface, SLOT(nextRun()));

    connect(runInterface, SIGNAL(solverFinished(const Pathfinder::PathfinderPerformanceMetric &,
                                         int)),
            this, SLOT(onSolverFinished(const Pathfinder::PathfinderPerformanceMetric&,
                               int)));

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
                                           int exitInt) {
    using RunInterface::RunnerReturnStatus;
    auto currentConfig = runQueue.front();
    auto exit = static_cast<RunnerReturnStatus>(exitInt);
    switch (exit) {
        case RunnerReturnStatus::RETURN_UNSOLVABLE:
            emit nextGrid();
            //TODO: save evaluation for unsolvable run somewhere
            break;
        case RunnerReturnStatus::RETURN_LAST_GRID_DONE:
            configIterator++;
            runProgressView->updateProgress(std::get<2>(currentConfig),
                                            static_cast<int>(configIterator /
                                                             std::get<0>(currentConfig).iterations.value()));
            handleNewConfigDemand();
            if (!finished) {
                currentConfig = runQueue.front();
                runProgressView->addNewConfig(std::get<2>(currentConfig));
                runProgressView->updateProgress(std::get<2>(currentConfig), 0);
                emit nextGrid();
            }
            break;
        case RunnerReturnStatus::RETURN_LAST_SOLVER_DONE:
            configIterator++;
            runProgressView->updateProgress(std::get<2>(currentConfig),
                                            static_cast<int>(configIterator * 100 /
                                                             std::get<0>(currentConfig).iterations.value()));
            emit nextGrid();
            break;
        case RunnerReturnStatus::RETURN_NORMAL:
            //TODO: save evaluation for run somewhere
            break;
    }
    if (finished) {
        //TODO: handle multi run finished
    } else if (runPaused) {
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green");
    } else {
        emit nextRun();
    }
}

void GUI::MultiRunDialog::handleNewConfigDemand() {
    runQueue.pop();
    if (!runQueue.empty()) {
        auto nextConfig = runQueue.front();
        emit sendNewData(std::get<0>(nextConfig), std::get<1>(nextConfig));
    } else {
        finished = true;
    }
    //TODO: update runView widget
}
