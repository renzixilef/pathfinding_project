#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QThread>

#include "runner.h"
#include "pathfinding.h"
#include "gui_run_progress_view.h"


Q_DECLARE_METATYPE(Pathfinder::PathfinderPerformanceMetric)

Q_DECLARE_METATYPE(RunInterface::RunGridConfig)

Q_DECLARE_METATYPE(Pathfinder::PathfinderStrategy)

Q_DECLARE_METATYPE(int32_t)


namespace GUI {
    class MultiRunDialog : public QDialog {
    Q_OBJECT
        using EvalMapType = std::map<RunInterface::RunGridConfig, std::tuple<std::unordered_map<
                Pathfinder::PathfinderStrategy, std::list<Pathfinder::PathfinderPerformanceMetric>>, uint32_t, QString>>;

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
        void closeEvent(QCloseEvent *event) override;

        void setupConnections();

        void handleNewConfigDemand();

        void toggleRunButtonHandler();

        void moveToEvaluationButtonHandler();

        inline void incrementUnsolvableCountForConfig(const auto &currentConfig) {
            std::get<1>(evalMap[std::get<0>(currentConfig)])++;
        }

        inline void pushBackPathfinderExitForCurrentConfig(
                const Pathfinder::PathfinderPerformanceMetric &pathfinderExit,
                const auto &currentConfig) {
            std::get<0>(evalMap[std::get<0>(currentConfig)])[pathfinderExit.strat].push_back(pathfinderExit);
        }

        inline void setDisplayableStringForCurrentConfig(const auto &currentConfig) {
            std::get<2>(evalMap[std::get<0>(currentConfig)]) = std::get<2>(currentConfig);
        }

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

        std::queue<std::tuple<RunInterface::RunGridConfig,
                std::list<Pathfinder::PathfinderStrategy>, QString>> &runQueue;

        EvalMapType evalMap;

    };

}