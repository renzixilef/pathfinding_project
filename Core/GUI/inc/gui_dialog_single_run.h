#pragma once

#include <QThread>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>

#include "runner.h"
#include "pathfinding.h"
#include "gui_grid.h"

Q_DECLARE_METATYPE(std::string)

//TODO: implement self definable start and end button for SingleRuns

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
        void serialize(const std::string&);

    public slots:
        void onGridFinished();

        void onStepFinished();

        void onSaveDone();

    private:
        void toggleRunButtonHandler();

        void setupConnections();

        void nextStepButtonHandler();

        void serializeButtonHandler();

        void toggleStartEndRedefinitionButtonHandler();

        bool runPaused = true;
        bool runFinished = false;

        RunInterface::SingleRun *runInterface;
        QThread *singleRunThread;

        Widgets::GridDrawerWidget *gridWidget;

        QPushButton *nextStepButton;
        QPushButton *toggleRunButton;
        QPushButton *serializeRunForDebugButton;
        QPushButton *toggleStartEndRedefinitionButton;

        QVBoxLayout *mainLayout;
        QHBoxLayout* gridWidgetLayout;
        QHBoxLayout *buttonLayout;

        QTimer* nextStepTimer;

    };
}