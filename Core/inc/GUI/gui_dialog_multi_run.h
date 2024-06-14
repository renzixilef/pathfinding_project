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

    private:
        RunInterface::MultiRun *runInterface;
        QThread *multiRunThread;

        QPushButton *playPauseButton;

        QVBoxLayout *mainLayout;

        QHBoxLayout *buttonLayout;

        std::queue<std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>>> runQueue;

    };

}