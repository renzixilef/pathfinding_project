#include "../inc/gui_tab_single_run.h"


GUI::SingleRunTab::SingleRunTab(QWidget *parent) :
        QWidget(parent),
        gridHeightSpinBox(new QSpinBox(this)),
        gridWidthSpinBox(new QSpinBox(this)),
        obstacleDensitySpinBox(new QDoubleSpinBox(this)),
        gridGeneratorAlgorithmComboBox(new QComboBox(this)),
        pathfindingAlgorithmComboBox(new QComboBox(this)),
        layout(new QFormLayout(this)),
        startRunButton(new QPushButton("Run", this)) {

    gridHeightSpinBox->setRange(50, UINT32_MAX);
    gridWidthSpinBox->setRange(50, UINT32_MAX);
    obstacleDensitySpinBox->setRange(0.0, 1.0);
    obstacleDensitySpinBox->setSingleStep(0.05);

    for(const auto& [k, v]: GridGenerator::obstacleGenStrategyToDisplayableText){
        gridGeneratorAlgorithmComboBox->addItem(QString::fromStdString(v), static_cast<uint8_t>(k));
    }
    //TODO: same code for pathfindingAlgorithmComboBox

    connect(startRunButton, &QPushButton::clicked, this, &SingleRunTab::startRun);

    layout->addRow("Grid Height", gridHeightSpinBox);
    layout->addRow("Grid Width", gridWidthSpinBox);
    layout->addRow("Obstacle Density", obstacleDensitySpinBox);
    layout->addRow("Grid Generator Algorithm", gridGeneratorAlgorithmComboBox);
    layout->addRow("Pathfinding Algorithm", pathfindingAlgorithmComboBox);
    layout->addRow(startRunButton);

    setLayout(layout);
}

void GUI::SingleRunTab::startRun() {
    //TODO: implement this functionality

}
