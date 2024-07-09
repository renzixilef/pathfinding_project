#pragma once

#include <map>
#include "runner.h"

namespace Application {
    class HeadlessRunner {
    public:
        using EvalMapType = std::map<RunInterface::RunGridConfig, std::tuple<std::unordered_map<
                Pathfinder::PathfinderStrategy, std::list<Pathfinder::PathfinderPerformanceMetric>>, uint32_t, QString>>;

        explicit HeadlessRunner(std::queue<std::pair<RunInterface::MultiRunConfig, QString>> &queue) : runQueue(
                queue) {}

        static QString generateConfigQString(const RunInterface::MultiRunConfig &configItem);

        virtual void headlessSolveAllNoWait();

        static QString generateEvalString(const auto& evalList, bool formatForGui = true);

    protected:
        inline void incrementUnsolvableCountForConfig(
                const std::pair<RunInterface::MultiRunConfig, QString> &currentConfig) {
            std::get<1>(evalMap[currentConfig.first.gridConfig])++;
        }

        inline void pushBackPathfinderExitForCurrentConfig(
                const Pathfinder::PathfinderPerformanceMetric &pathfinderExit,
                const std::pair<RunInterface::MultiRunConfig, QString> &currentConfig) {
            std::get<0>(evalMap[currentConfig.first.gridConfig])[pathfinderExit.strat].push_back(pathfinderExit);
        }

        inline void setDisplayableStringForCurrentConfig(
                const std::pair<RunInterface::MultiRunConfig, QString> &currentConfig) {
            std::get<2>(evalMap[currentConfig.first.gridConfig]) = currentConfig.second;
        }

        EvalMapType evalMap;

        std::queue<std::pair<RunInterface::MultiRunConfig, QString>> runQueue;

    private:
        void headlessSolveOneConfigNoWait(std::pair<RunInterface::MultiRunConfig, QString> &itemConfig);

        void headlessPrintEvaluation();

        static void updateProgressBar(uint16_t percent);

        static uint32_t getConsoleWidth();
    };
}