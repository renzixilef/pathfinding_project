#include "GUI/gui_config_form.h"

GUI::ConfigFormParent::ConfigFormParent(QWidget *parent) : gridHeightSpinBox(new QSpinBox(this)),
                                                           gridWidthSpinBox(new QSpinBox(this)),
                                                           obstacleDensitySpinBox(new QDoubleSpinBox(this)),
                                                           minStartEndDistanceSpinBox(new QDoubleSpinBox(this)),
                                                           gridGeneratorAlgorithmComboBox(new QComboBox(this)),
                                                           layout(new QFormLayout(this)) {
    using GridGenerator::ObstacleGenStrategyParser;
    for (const auto &[k, v]:
            ObstacleGenStrategyParser::obstacleGenStrategyToDisplayableText) {
        gridGeneratorAlgorithmComboBox->addItem(QString::fromStdString(v), static_cast<uint8_t>(k));
    }
    layout->addRow("Grid Height", gridHeightSpinBox);
    layout->addRow("Grid Width", gridWidthSpinBox);
    layout->addRow("Obstacle Density", obstacleDensitySpinBox);
    layout->addRow("Min. Distance Start/End [% short grid side]", minStartEndDistanceSpinBox);
    layout->addRow("Grid Generator Algorithm", gridGeneratorAlgorithmComboBox);
}

GUI::SingleConfigForm::SingleConfigForm(QWidget *parent) : ConfigFormParent(this),
                                                           pathfindingAlgorithmComboBox(new QComboBox(this)) {
    using Pathfinder::PathfinderStrategyParser;
    gridHeightSpinBox->setRange(20, 100);
    gridWidthSpinBox->setRange(20, 100);
    obstacleDensitySpinBox->setRange(0.0, 0.7);
    obstacleDensitySpinBox->setSingleStep(0.05);
    minStartEndDistanceSpinBox->setRange(0.0, 0.9);
    minStartEndDistanceSpinBox->setSingleStep(0.05);

    for (const auto &[k, v]:
            PathfinderStrategyParser::pathfindingStrategyToDisplayableText) {
        pathfindingAlgorithmComboBox->addItem(QString::fromStdString(v), static_cast<uint8_t>(k));
    }

    layout->addRow("Pathfinding Algorithm", pathfindingAlgorithmComboBox);
    setLayout(layout);
}

std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>>
GUI::SingleConfigForm::getFormParams() {
    RunInterface::RunGridConfig thisGridConfig = {
            static_cast<uint32_t>(gridWidthSpinBox->value()),
            static_cast<uint32_t>(gridHeightSpinBox->value()),
            static_cast<float>(obstacleDensitySpinBox->value()),
            static_cast<float>(minStartEndDistanceSpinBox->value()),
            static_cast<GridGenerator::ObstacleGenStrategy>(
                    gridGeneratorAlgorithmComboBox->currentData().toUInt())
    };
    std::list<Pathfinder::PathfinderStrategy> thisPathfinderStratList;
    thisPathfinderStratList.push_back(static_cast<Pathfinder::PathfinderStrategy>(
                                              pathfindingAlgorithmComboBox->currentData().toUInt()));
    return std::make_pair(thisGridConfig, thisPathfinderStratList);

}
