#include "gui_live_evaluator.h"

GUI::Widgets::LiveEvaluatorWidget::LiveEvaluatorWidget(const Pathfinder::PathfindingParent &solver, QWidget *parent) :
        QWidget(parent),
        solver(solver),
        capsuleLayout(new QVBoxLayout(this)),
        layout(new QGridLayout()) {
    capsuleLayout->addLayout(layout);
    capsuleLayout->addStretch(1);
    updateMetrics();
}

void GUI::Widgets::LiveEvaluatorWidget::addMetric(const QString &id, const QString &val) {
    auto identifierLabel = new QLabel(id, this);
    QFont font = identifierLabel->font();
    font.setBold(true);
    identifierLabel->setFont(font);
    auto valueLabel = new QLabel(val, this);
    uint16_t i = labels.size();

    layout->addWidget(identifierLabel, i, 0, Qt::AlignRight);
    layout->addWidget(valueLabel, i, 1, Qt::AlignLeft);
    labels[id] = valueLabel;
}

void GUI::Widgets::LiveEvaluatorWidget::updateMetrics() {
    const QString stepLabel = "Step [-] ";
    const QString totalTimeLabel = "Total Time [ms] ";
    const QString lastStepTimeLabel = "Last Step Time [us] ";
    const QString avgStepTimeLabel = "Avg Step Time [us] ";
    const QString visitedCellsLabel = "Visited Cells [-] ";
    const QString closedCellsLabel = "Closed Cells [-] ";
    const QString pathCellsLabel = "Path Cells [-] ";

    Pathfinder::PathfinderPerformanceMetric currentPerformanceMetric = solver.getPerformanceMetric();

    if (labels.contains(stepLabel))
        labels[stepLabel]->setText(QString::number(currentPerformanceMetric.stepCount));
    else 
        addMetric(stepLabel, QString::number(currentPerformanceMetric.stepCount));

    if (labels.contains(totalTimeLabel))
        labels[totalTimeLabel]->setText(QString::number(currentPerformanceMetric.solvingUSeconds/1000, 'f', 3));
    else 
        addMetric(totalTimeLabel, QString::number(currentPerformanceMetric.solvingUSeconds/1000, 'f', 3));

    auto stepTime = QString::number(currentPerformanceMetric.solvingUSeconds - lastSolvingUSeconds);
    if (labels.contains(lastStepTimeLabel)) 
        labels[lastStepTimeLabel]->setText(stepTime);
    else 
        addMetric(lastStepTimeLabel, stepTime);
    lastSolvingUSeconds = currentPerformanceMetric.solvingUSeconds;
    
    if (labels.contains(avgStepTimeLabel)) 
        labels[avgStepTimeLabel]->setText(QString::number(currentPerformanceMetric.avgUSecondsPerStep, 'f', 3));
    else 
        addMetric(avgStepTimeLabel, QString::number(currentPerformanceMetric.avgUSecondsPerStep, 'f', 3));
    
    if (labels.contains(visitedCellsLabel)) 
        labels[visitedCellsLabel]->setText(QString::number(currentPerformanceMetric.visitedCells));
    else 
        addMetric(visitedCellsLabel, QString::number(currentPerformanceMetric.visitedCells));
    
    if (labels.contains(closedCellsLabel))
        labels[closedCellsLabel]->setText(QString::number(currentPerformanceMetric.stepCount));
    else 
        addMetric(closedCellsLabel, QString::number(currentPerformanceMetric.stepCount));

    if (labels.contains(pathCellsLabel))
        labels[pathCellsLabel]->setText(QString::number(currentPerformanceMetric.pathCells));
    else
        addMetric(pathCellsLabel, QString::number(currentPerformanceMetric.pathCells));

    

}
