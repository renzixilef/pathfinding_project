#include "GUI/gui_tab_single_run.h"
#include "Pathfinder/pathfinding.h"
#include "GridGenerator/obstacle_gen.h"
#include "../../inc/GUI/gui_dialog_single_run.h"


GUI::SingleRunTab::SingleRunTab(QWidget *parent) :
        QWidget(parent),
        gridHeightSpinBox(new QSpinBox(this)),
        gridWidthSpinBox(new QSpinBox(this)),
        obstacleDensitySpinBox(new QDoubleSpinBox(this)),
        minStartEndDistanceSpinBox(new QDoubleSpinBox(this)),
        gridGeneratorAlgorithmComboBox(new QComboBox(this)),
        pathfindingAlgorithmComboBox(new QComboBox(this)),
        layout(new QFormLayout(this)),
        startRunButton(new QPushButton("Run", this)) {
    using GridGenerator::ObstacleGenStrategyParser;
    using Pathfinder::PathfinderStrategyParser;
    gridHeightSpinBox->setRange(50, UINT32_MAX);
    gridWidthSpinBox->setRange(50, UINT32_MAX);
    obstacleDensitySpinBox->setRange(0.0, 1.0);
    obstacleDensitySpinBox->setSingleStep(0.05);
    minStartEndDistanceSpinBox->setRange(0.0, 1.0);
    minStartEndDistanceSpinBox->setSingleStep(0.05);

    for (const auto &[k, v]:
            ObstacleGenStrategyParser::obstacleGenStrategyToDisplayableText) {
        gridGeneratorAlgorithmComboBox->addItem(QString::fromStdString(v), static_cast<uint8_t>(k));
    }
    for (const auto &[k, v]:
            PathfinderStrategyParser::pathfindingStrategyToDisplayableText) {
        pathfindingAlgorithmComboBox->addItem(QString::fromStdString(v), static_cast<uint8_t>(k));
    }

    connect(startRunButton, &QPushButton::clicked, this, &SingleRunTab::startRun);

    layout->addRow("Grid Height", gridHeightSpinBox);
    layout->addRow("Grid Width", gridWidthSpinBox);
    layout->addRow("Obstacle Density", obstacleDensitySpinBox);
    layout->addRow("Min. Distance Start/End [% short grid side]", minStartEndDistanceSpinBox);
    layout->addRow("Grid Generator Algorithm", gridGeneratorAlgorithmComboBox);
    layout->addRow("Pathfinding Algorithm", pathfindingAlgorithmComboBox);
    layout->addRow(startRunButton);

    setLayout(layout);
}

void GUI::SingleRunTab::startRun() {
    QDialog* singleRunDialog = new GUI::SingleRunDialog(RunInterface::RunGridConfig{
        static_cast<uint32_t>(gridWidthSpinBox->value()),
        static_cast<uint32_t>(gridHeightSpinBox->value()),
        static_cast<float>(obstacleDensitySpinBox->value()),
        static_cast<float>(minStartEndDistanceSpinBox->value()),
        static_cast<GridGenerator::ObstacleGenStrategy>(gridGeneratorAlgorithmComboBox->currentData().toUInt())},
                                                       static_cast<Pathfinder::PathfinderStrategy>(pathfindingAlgorithmComboBox->currentData().toUInt()));
    singleRunDialog->exec();

    //TODO: implement this functionality

}
