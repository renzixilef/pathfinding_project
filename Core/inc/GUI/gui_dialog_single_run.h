#pragma once

#include <QThread>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>

#include "RunInterface/runner.h"
#include "GUI/widgets/gui_grid.h"

namespace GUI {
    class SingleRunDialog : public QDialog {
    Q_OBJECT

    public:
        SingleRunDialog(const RunInterface::RunGridConfig& config,
                        const Pathfinder::PathfinderStrategy& strat,
                        QWidget *parent = nullptr);

        ~SingleRunDialog() override {
            singleRunThread->quit();
            singleRunThread->wait();
        }

    signals:

        void nextStep();
        void resetRun();

    public slots:
        void onGridFinished();

        void onStepFinished();

    private:
        void toggleRunButtonHandler();

        void setupConnections();

        void nextStepButtonHandler();

        bool runPaused = true;
        bool runFinished = false;

        RunInterface::SingleRun *runInterface;
        QThread *singleRunThread;

        Widgets::GridDrawerWidget *gridWidget;

        QPushButton *nextStepButton;
        QPushButton *toggleRunButton;

        QVBoxLayout *mainLayout;
        QHBoxLayout* gridWidgetLayout;
        QHBoxLayout *buttonLayout;

        QTimer* nextStepTimer;

    };
}