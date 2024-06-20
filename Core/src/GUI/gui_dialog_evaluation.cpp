#include "GUI/gui_dialog_evaluation.h"

GUI::EvaluationDialog::EvaluationDialog(const GUI::EvaluationDialog::EvalMapType &map, QWidget *parent) :
        QDialog(parent),
        evalMap(map),
        overviewScrollArea(new QScrollArea(this)),
        scrollWidget(new QWidget()),
        mainLayout(new QVBoxLayout()),
        scrollAreaLayout(new QVBoxLayout()) {
    overviewScrollArea->setWidgetResizable(true);
    auto &stratToText =  Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText;

    uint16_t maxLabelWidth = 0;

    for (auto &evalPair: evalMap) {
        auto *evalLabel = new QLabel();
        QString labelText = "<b>" + std::get<2>(evalPair.second) + "</b><br/>";
        uint16_t labelWidth = evalLabel->fontMetrics().boundingRect(labelText).width();
        if(labelWidth > maxLabelWidth) maxLabelWidth = labelWidth;
        labelText += "UNSOLVABLE GRIDS GENERATED: " + QString::number(std::get<1>(evalPair.second)) + "<br/>";
        for(auto& pathfinderEval: std::get<0>(evalPair.second)){
            labelText += "Pathfinder: " +
                    QString::fromStdString(stratToText.at(pathfinderEval.first));
            labelText+= generateEvalString(pathfinderEval.second);
        }
        evalLabel->setText(labelText);
        evalLabel->setWordWrap(true);
        evalLabel->setAlignment(Qt::AlignTop);
        scrollAreaLayout->addWidget(evalLabel);
    }
    scrollWidget->setMinimumWidth(maxLabelWidth);
    overviewScrollArea->setMinimumWidth(maxLabelWidth);

    scrollWidget->setLayout(scrollAreaLayout);
    overviewScrollArea->setWidget(scrollWidget);

    mainLayout->addWidget(overviewScrollArea);
    setLayout(mainLayout);

}

QString GUI::EvaluationDialog::generateEvalString(const auto& evalList) {
    QString evalString{};
    double absoluteSolvingUSeconds = 0;
    uint32_t totalClosedCells = 0;
    uint32_t totalVisitedCells = 0;
    uint32_t absoluteStepsTaken = 0;
    uint32_t numberOfGrids = evalList.size();

    for(const auto& pathfinderReturn: evalList){
        absoluteSolvingUSeconds += pathfinderReturn.solvingUSeconds;
        absoluteStepsTaken += pathfinderReturn.stepCount;
        totalClosedCells += pathfinderReturn.closedCells;
        totalVisitedCells += pathfinderReturn.visitedCells;
    }
    double avgSolvingMSeconds = (absoluteSolvingUSeconds/1000)/numberOfGrids;
    double avgUSecondsPerStep = absoluteSolvingUSeconds/absoluteStepsTaken;
    double absoluteSolvingSeconds = absoluteSolvingUSeconds/(1000*1000);
    double avgStepsTaken = static_cast<double>(absoluteStepsTaken)/numberOfGrids;

    evalString += "<pre>      ABSOLUTE SOLVING TIME [s]: " + QString::number(absoluteSolvingSeconds) + "</pre>";
    evalString += "<pre>      AVG SOLVING TIME [ms]: " + QString::number(avgSolvingMSeconds) + "</pre>";
    evalString += "<pre>      AVG STEP TIME [us]: " + QString::number(avgUSecondsPerStep) + "</pre>";
    evalString += "<pre>      AVG STEP COUNT [-]: " + QString::number(avgStepsTaken) + "</pre>";
    evalString += "<pre>      TOTAL CLOSED CELLS [-]: " + QString::number(totalClosedCells) + "</pre>";
    evalString += "<pre>      TOTAL VISITED CELLS [-]: " + QString::number(totalVisitedCells) + "</pre><br/>";
    return evalString;


}
