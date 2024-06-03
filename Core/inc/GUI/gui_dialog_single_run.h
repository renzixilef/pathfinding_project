#pragma once

#include <QThread>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>

#include "RunInterface/runner.h"
#include "GUI/gui_grid.h"

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
        void onGridFinished();

        void onStepFinished();

    private:
        void toggleRunButtonHandler();

        void nextStepButtonHandler();

        bool runPaused = true;

        RunInterface::SingleRun *runInterface;
        QThread *singleRunThread;

        GridDrawerWidget *gridWidget;

        QPushButton *nextStepButton;
        QPushButton *toggleRunButton;

        QVBoxLayout *mainLayout;
        QHBoxLayout* gridWidgetLayout;
        QHBoxLayout *buttonLayout;

        QTimer* nextStepTimer;

    };
}