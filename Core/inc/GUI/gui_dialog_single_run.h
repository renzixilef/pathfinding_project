#pragma once

#include <QThread>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

#include "../RunInterface/runner.h"

namespace GUI {
    class SingleRunDialog : public QDialog {
    Q_OBJECT

    public:
        SingleRunDialog(RunInterface::RunGridConfig config,
                        Pathfinder::PathfinderStrategy strat,
                        QWidget *parent = nullptr);
        ~SingleRunDialog() override {
            singleRunThread->quit();
            singleRunThread->wait();
        }

    signals:

        void nextStep();

    public slots:
        void onStepFinished();

    private:
        RunInterface::SingleRun *runInterface;

        QThread *singleRunThread;
        QPushButton *nextStepButton;
        QPushButton *startRunNoWaitButton;


    };
}