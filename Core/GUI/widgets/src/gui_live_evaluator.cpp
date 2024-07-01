#include "gui_live_evaluator.h"

GUI::Widgets::LiveEvaluatorWidget::LiveEvaluatorWidget(const Pathfinder::PathfindingParent &solver, QWidget *parent) :
        QWidget(parent),
        solver(solver),
        layout(new QGridLayout(this)) {
    Pathfinder::PathfinderPerformanceMetric currentPerformanceMetric = solver.getPerformanceMetric();

    addMetric("Step [-]", QString::number(currentPerformanceMetric.stepCount));
    addMetric("Total Time [us] ", QString::number(currentPerformanceMetric.solvingUSeconds));
    addMetric("Last Step Time [us] ", QString::number(currentPerformanceMetric.solvingUSeconds-lastSolvingUSeconds));
    lastSolvingUSeconds = currentPerformanceMetric.solvingUSeconds;
    addMetric("Avg Step Time [us] ", QString::number(currentPerformanceMetric.avgUSecondsPerStep));
    addMetric("Visited Cells [-] ", QString::number(currentPerformanceMetric.visitedCells));
    addMetric("Closed Cells [-] ", QString::number(currentPerformanceMetric.closedCells));
    addMetric("Path Cells [-]", QString::number(currentPerformanceMetric.pathCells));

}

void GUI::Widgets::LiveEvaluatorWidget::addMetric(const QString &id, const QString &val) {
    auto identifierLabel = new QLabel(id, this);
    auto valueLabel = new QLabel(val, this);
    uint16_t i = labels.size();

    layout->addWidget(identifierLabel, i, 0, Qt::AlignRight);
    layout->addWidget(valueLabel, i, 1, Qt::AlignLeft);
    labels[id] = valueLabel;
}
