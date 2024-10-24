#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QThread>

#include "runner.h"
#include "pathfinding.h"
#include "gui_run_progress_view.h"
#include "headless_multi_run.h"


Q_DECLARE_METATYPE(Pathfinder::PathfinderPerformanceMetric)

Q_DECLARE_METATYPE(RunInterface::RunGridConfig)

Q_DECLARE_METATYPE(RunInterface::MultiRunConfig)

Q_DECLARE_METATYPE(Pathfinder::PathfinderStrategy)

Q_DECLARE_METATYPE(int32_t)


namespace GUI {
    class MultiRunDialog : public QDialog, public Application::HeadlessRunner {
    Q_OBJECT

    public:
        explicit MultiRunDialog(std::queue<std::pair<RunInterface::MultiRunConfig, QString>> &queue,
                                QWidget *parent = nullptr);

        ~MultiRunDialog() override {
            multiRunThread->quit();
            multiRunThread->wait();
        }

        void headlessSolveAllNoWait() override {};

    public slots:

        void onSolverFinished(const Pathfinder::PathfinderPerformanceMetric &pathfinderExit,
                              int32_t exitInt);

    signals:

        void nextGrid();

        void nextRun();

        void sendNewData(const RunInterface::MultiRunConfig &);

    private:
        void closeEvent(QCloseEvent *event) override;

        void setupConnections();

        void handleNewConfigDemand();

        void toggleRunButtonHandler();

        void moveToEvaluationButtonHandler();

        void updateGUIAfterFinishedRun();

        bool runPaused = true;
        bool finished = false;

        uint32_t gridIterator = 0;

        bool shouldRepeatUnsolvables;

        RunInterface::MultiRun *runInterface;
        QThread *multiRunThread;

        QPushButton *toggleRunButton;
        QPushButton *moveToEvaluationButton;

        QVBoxLayout *mainLayout;

        QHBoxLayout *buttonLayout;

        Widgets::RunProgressView *runProgressView;

    };

}