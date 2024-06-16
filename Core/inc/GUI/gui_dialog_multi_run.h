#pragma once

#include <QDialog>
#include <QBoxLayout>
#include <QThread>

#include "RunInterface/runner.h"
#include "widgets/gui_run_progress_view.h"

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

        void onSolverFinished(std::optional<Pathfinder::PathfinderPerformanceMetric> pathfinderExit,
                              RunInterface::RunnerReturnStatus exit);

    signals:
        void nextGrid();
        void nextRun();
        void sendNewData(const RunInterface::RunGridConfig&,
                         const std::list<Pathfinder::PathfinderStrategy>&);

    private:
        void setupConnections();
        void handleNewConfigDemand();
        void toggleRunButtonHandler();

        bool runPaused = true;
        bool finished = false;

        uint32_t configIterator = 0;

        bool shouldRepeatUnsolvables = true;

        RunInterface::MultiRun *runInterface;
        QThread *multiRunThread;

        QPushButton *toggleRunButton;

        QVBoxLayout *mainLayout;

        QHBoxLayout *buttonLayout;

        Widgets::RunProgressView* runProgressView;

        std::queue<std::tuple<RunInterface::RunGridConfig,
                std::list<Pathfinder::PathfinderStrategy>, QString>>& runQueue;

    };

}