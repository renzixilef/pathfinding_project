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
        explicit MultiRunDialog(std::queue<std::pair<RunInterface::MultiRunConfig, QString>> &queue,
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

        void sendNewData(const RunInterface::MultiRunConfig &);

    private:
        void closeEvent(QCloseEvent *event) override;

        void setupConnections();

        void handleNewConfigDemand();

        void toggleRunButtonHandler();

        void moveToEvaluationButtonHandler();

        void updateGUIAfterFinishedRun();

        inline void incrementUnsolvableCountForConfig(
                const std::pair<RunInterface::MultiRunConfig, QString> &currentConfig) {
            std::get<1>(evalMap[currentConfig.first.config])++;
        }

        inline void pushBackPathfinderExitForCurrentConfig(
                const Pathfinder::PathfinderPerformanceMetric &pathfinderExit,
                const std::pair<RunInterface::MultiRunConfig, QString> &currentConfig) {
            std::get<0>(evalMap[currentConfig.first.config])[pathfinderExit.strat].push_back(pathfinderExit);
        }

        inline void setDisplayableStringForCurrentConfig(
                const std::pair<RunInterface::MultiRunConfig, QString> &currentConfig) {
            std::get<2>(evalMap[currentConfig.first.config]) = currentConfig.second;
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

        std::queue<std::pair<RunInterface::MultiRunConfig, QString>> &runQueue;

        EvalMapType evalMap;
    };

}