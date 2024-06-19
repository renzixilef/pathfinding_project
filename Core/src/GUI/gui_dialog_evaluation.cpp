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

    for (auto &evalPair: evalMap) {
        QLabel *evalLabel = new QLabel();
        QString labelText = "<b>" + std::get<2>(evalPair.second) + "<b>\n";
        labelText += "UNSOLVABLE GRIDS GENERATED: " + QString::number(std::get<1>(evalPair.second)) + "\n";
        for(auto& pathfinderEval: std::get<0>(evalPair.second)){
            labelText += "Pathfinder: " +
                    QString::fromStdString(stratToText.at(pathfinderEval.first)) + "\n";
            labelText+= generateEvalString(pathfinderEval.second);
        }
        evalLabel->setText(labelText);
        //evalLabel->setWordWrap(true);
        evalLabel->setAlignment(Qt::AlignTop);
        scrollAreaLayout->addWidget(evalLabel);
    }
    scrollWidget->setLayout(scrollAreaLayout);
    overviewScrollArea->setWidget(scrollWidget);

    mainLayout->addWidget(overviewScrollArea);
    setLayout(mainLayout);

}

QString GUI::EvaluationDialog::generateEvalString(const auto& evalList) {
    QString evalString{};
    double absoluteSolvingSeconds = 0;
    uint32_t totalClosedCells = 0;
    uint32_t totalVisitedCells = 0;
    uint32_t absoluteStepsTaken = 0;
    uint32_t numberOfGrids = evalList.size();

    for(const auto& pathfinderReturn: evalList){
        absoluteSolvingSeconds += pathfinderReturn.solvingUSeconds/1000;
        absoluteStepsTaken += pathfinderReturn.stepCount;
        totalClosedCells += pathfinderReturn.closedCells;
        totalVisitedCells += pathfinderReturn.visitedCells;
    }
    double avgSolvingUSeconds = absoluteSolvingSeconds*1000/numberOfGrids;
    double avgUSecondsPerStep = absoluteSolvingSeconds*1000/absoluteStepsTaken;
    double avgStepsTaken = static_cast<double>(absoluteStepsTaken)/numberOfGrids;

    evalString += "\t ABSOLUTE SOLVING TIME [s]: " + QString::number(absoluteSolvingSeconds) + "\n";
    evalString += "\t AVG SOLVING TIME [us]: " + QString::number(avgSolvingUSeconds) + "\n";
    evalString += "\t AVG STEP TIME [us]: " + QString::number(avgUSecondsPerStep) + "\n";
    evalString += "\t AVG STEP COUNT [-]: " + QString::number(avgStepsTaken) + "\n";
    evalString += "\t TOTAL CLOSED CELLS [-]: " + QString::number(totalClosedCells) + "\n";
    evalString += "\t TOTAL VISITED CELLS [-]: " + QString::number(totalVisitedCells) + "\n";
    return evalString;


}
