#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QMap>
#include <QLabel>

#include "pathfinding.h"

namespace GUI::Widgets {
    class LiveEvaluatorWidget : public QWidget {
    Q_OBJECT
    public:
        explicit LiveEvaluatorWidget(const Pathfinder::PathfindingParent &solver, QWidget *parent = nullptr);

        void updatePerformanceMetric();

    private:
        void addMetric(const QString &id, const QString &val);

        const Pathfinder::PathfindingParent &solver;

        QGridLayout * layout;
        QMap<QString, QLabel*> labels;

        double lastSolvingUSeconds;


    };
}