#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>

#include "runner.h"
#include "pathfinding.h"

namespace GUI {
    class EvaluationDialog : public QDialog {
    Q_OBJECT
        using EvalMapType = std::map<RunInterface::RunGridConfig, std::tuple<std::unordered_map<
                Pathfinder::PathfinderStrategy, std::list<Pathfinder::PathfinderPerformanceMetric>>, uint32_t, QString>>;

    public:
        explicit EvaluationDialog(const EvalMapType& map, QWidget* parent);

    private:

        const EvalMapType& evalMap;

        QVBoxLayout* mainLayout;
        QVBoxLayout* scrollAreaLayout;
        QScrollArea* overviewScrollArea;
        QWidget* scrollWidget;


    };
}

