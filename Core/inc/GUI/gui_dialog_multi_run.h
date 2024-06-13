#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QThread>

#include "RunInterface/runner.h"

namespace GUI {
    class MultiRunDialog : public QDialog {
    Q_OBJECT
    public:
        MultiRunDialog(const RunInterface::RunGridConfig& config,
                       const std::list<Pathfinder::PathfinderStrategy>& strats,
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

    };

}