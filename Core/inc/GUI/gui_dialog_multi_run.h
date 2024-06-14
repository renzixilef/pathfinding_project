#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QThread>

#include "RunInterface/runner.h"

namespace GUI {
    class MultiRunDialog : public QDialog {
    Q_OBJECT
    public:
        explicit MultiRunDialog(std::queue<std::pair<RunInterface::RunGridConfig,
                std::list<Pathfinder::PathfinderStrategy>>> queue,
                                QWidget *parent = nullptr);

        ~MultiRunDialog() override {
            multiRunThread->quit();
            multiRunThread->wait();
        }

    public slots:

        void onGridFinished();

        void onSolverFinished();

        void onNewConfigDemand();


    signals:

        void nextRun();
        void sendNewData(const RunInterface::RunGridConfig &thisConfig,
                         const std::list<Pathfinder::PathfinderStrategy> &thisStrats);

    private:
        void setupConnections();

        void toggleRunButtonHandler();

        bool runPaused = true;

        RunInterface::MultiRun *runInterface;
        QThread *multiRunThread;

        QPushButton *toggleRunButton;

        QVBoxLayout *mainLayout;

        QHBoxLayout *buttonLayout;

        std::queue<std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>>> runQueue;

    };

}