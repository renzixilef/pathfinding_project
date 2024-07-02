#pragma once

#include <QThread>
#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>
#include <QToolButton>
#include <QFutureWatcher>

#include "runner.h"
#include "pathfinding.h"
#include "gui_grid.h"
#include "gui_live_evaluator.h"

Q_DECLARE_METATYPE(std::string)

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
        void startEndChanged();

    public slots:
        void onGridFinished();

        void onStepFinished();

        void onSaveDone();

    private:
        void closeEvent(QCloseEvent* event) override;

        void toggleRunButtonHandler();

        void setupConnections();

        void nextStepButtonHandler();

        void serializeButtonHandler();

        void exportVideoHandler();

        void toggleStartEndRedefinitionButtonHandler();

        bool runPaused = true;
        bool runFinished = false;

        RunInterface::SingleRun *runInterface;
        QThread *singleRunThread;

        Widgets::GridDrawerWidget *gridWidget;
        Widgets::LiveEvaluatorWidget *liveEvaluator;

        QPushButton *nextStepButton;
        QPushButton *toggleRunButton;
        QToolButton *exportRunMenuButton;
        QAction *exportVideoAction;
        QAction *debugSerializeAction;
        QPushButton *toggleStartEndRedefinitionButton;

        QVBoxLayout *mainLayout;
        QHBoxLayout* gridWidgetLayout;
        QHBoxLayout *buttonLayout;

        QTimer* nextStepTimer;

        QFuture<void> exportVideoFuture;
        QFutureWatcher<void> exportVideoWatcher;

    };
}