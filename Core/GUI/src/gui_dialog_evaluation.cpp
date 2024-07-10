#include "gui_dialog_evaluation.h"
#include "headless_multi_run.h"

GUI::EvaluationDialog::EvaluationDialog(const GUI::EvaluationDialog::EvalMapType &map, QWidget *parent) :
        QDialog(parent),
        evalMap(map),
        overviewScrollArea(new QScrollArea(this)),
        scrollWidget(new QWidget()),
        mainLayout(new QVBoxLayout()),
        scrollAreaLayout(new QVBoxLayout()) {
    setWindowTitle("Pathfinder 1.0 - Multi Run - Evaluation");

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
                    QString::fromStdString(stratToText.at(pathfinderEval.first)) + "\n";
            labelText+= Application::HeadlessRunner::generateEvalString(pathfinderEval.second);
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

