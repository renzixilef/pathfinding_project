#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QThread>

#include "RunInterface/runner.h"
#include "widgets/gui_run_progress_view.h"

Q_DECLARE_METATYPE(Pathfinder::PathfinderPerformanceMetric)

Q_DECLARE_METATYPE(RunInterface::RunGridConfig)

Q_DECLARE_METATYPE(Pathfinder::PathfinderStrategy)

namespace GUI {
    class MultiRunDialog : public QDialog {
    Q_OBJECT
    public:
        explicit MultiRunDialog(std::queue<std::tuple<RunInterface::RunGridConfig,
                std::list<Pathfinder::PathfinderStrategy>, QString>> &queue,
                                QWidget *parent = nullptr);

        ~MultiRunDialog() override {
            multiRunThread->quit();
            multiRunThread->wait();
        }

    public slots:

        void onSolverFinished(const Pathfinder::PathfinderPerformanceMetric &pathfinderExit,
                              int32_t exitInt);

    signals:

        void nextGrid();

        void nextRun();

        void sendNewData(const RunInterface::RunGridConfig &,
                         const std::list<Pathfinder::PathfinderStrategy> &);

    private:
        void setupConnections();

        void handleNewConfigDemand();

        void toggleRunButtonHandler();

        void moveToEvaluationButtonHandler();

        bool runPaused = true;
        bool finished = false;

        uint32_t configIterator = 0;

        bool shouldRepeatUnsolvables = true;

        RunInterface::MultiRun *runInterface;
        QThread *multiRunThread;

        QPushButton *toggleRunButton;
        QPushButton *moveToEvaluationButton;

        QVBoxLayout *mainLayout;

        QHBoxLayout *buttonLayout;

        Widgets::RunProgressView *runProgressView;

        std::queue<std::tuple<RunInterface::RunGridConfig,
                std::list<Pathfinder::PathfinderStrategy>, QString>> &runQueue;

        std::map<RunInterface::RunGridConfig,
                std::pair<std::unordered_map<Pathfinder::PathfinderStrategy,
                        std::list<Pathfinder::PathfinderPerformanceMetric>>,
                        uint32_t>> evalMap;

    };

}