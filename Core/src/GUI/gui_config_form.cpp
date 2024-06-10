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

void GUI::ConfigFormParent::resetForm() {
    gridHeightSpinBox->setValue(20);
    gridWidthSpinBox->setValue(20);
    obstacleDensitySpinBox->setValue(0.0);
    minStartEndDistanceSpinBox->setValue(0.0);

    gridGeneratorAlgorithmComboBox->setCurrentIndex(0);
}

void GUI::ConfigFormParent::disable() {
    gridHeightSpinBox->setDisabled(true);
    gridWidthSpinBox->setDisabled(true);
    obstacleDensitySpinBox->setDisabled(true);
    minStartEndDistanceSpinBox->setDisabled(true);
    gridGeneratorAlgorithmComboBox->setDisabled(true);
}

void GUI::ConfigFormParent::enable() {
    gridHeightSpinBox->setDisabled(false);
    gridWidthSpinBox->setDisabled(false);
    obstacleDensitySpinBox->setDisabled(false);
    minStartEndDistanceSpinBox->setDisabled(false);
    gridGeneratorAlgorithmComboBox->setDisabled(false);
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

void GUI::SingleConfigForm::resetForm() {
    ConfigFormParent::resetForm();
    pathfindingAlgorithmComboBox->setCurrentIndex(0);
}

void GUI::SingleConfigForm::enable() {
    ConfigFormParent::enable();
    pathfindingAlgorithmComboBox->setDisabled(false);
}

void GUI::SingleConfigForm::disable() {
    ConfigFormParent::disable();
    pathfindingAlgorithmComboBox->setDisabled(true);
}

GUI::MultiConfigForm::MultiConfigForm(QWidget *parent) : ConfigFormParent(parent),
                                                         pathfindingAlgorithmListWidget(new QListWidget(this)) {
    using Pathfinder::PathfinderStrategyParser;
    gridHeightSpinBox->setRange(20, 100);
    gridWidthSpinBox->setRange(20, 100);
    obstacleDensitySpinBox->setRange(0.0, 0.7);
    obstacleDensitySpinBox->setSingleStep(0.05);
    minStartEndDistanceSpinBox->setRange(0.0, 0.9);
    minStartEndDistanceSpinBox->setSingleStep(0.05);

    for (const auto &[k, v]:
            PathfinderStrategyParser::pathfindingStrategyToDisplayableText) {
        auto *item = new QListWidgetItem(QString::fromStdString(v));
        item->setData(Qt::UserRole, static_cast<uint8_t>(k));
        pathfindingAlgorithmListWidget->addItem(item);
    }

    layout->addRow("Pathfinding Algorithm", pathfindingAlgorithmListWidget);
    setLayout(layout);
}

void GUI::MultiConfigForm::enable() {
    ConfigFormParent::enable();
    pathfindingAlgorithmListWidget->setDisabled(false);
}

void GUI::MultiConfigForm::disable() {
    ConfigFormParent::disable();
    pathfindingAlgorithmListWidget->setDisabled(true);
}

void GUI::MultiConfigForm::resetForm() {
    ConfigFormParent::resetForm();
    pathfindingAlgorithmListWidget->clearSelection();
}

std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>>
GUI::MultiConfigForm::getFormParams() {
    RunInterface::RunGridConfig thisGridConfig = {
            static_cast<uint32_t>(gridWidthSpinBox->value()),
            static_cast<uint32_t>(gridHeightSpinBox->value()),
            static_cast<float>(obstacleDensitySpinBox->value()),
            static_cast<float>(minStartEndDistanceSpinBox->value()),
            static_cast<GridGenerator::ObstacleGenStrategy>(
                    gridGeneratorAlgorithmComboBox->currentData().toUInt())
    };

    QList<QListWidgetItem*> selectedItems = pathfindingAlgorithmListWidget->selectedItems();
    std::list<Pathfinder::PathfinderStrategy> thisPathfinderStratList;
    for (auto *item: selectedItems) {
        thisPathfinderStratList.push_back(
                static_cast<Pathfinder::PathfinderStrategy>(item->data(Qt::UserRole).toUInt()));
    }
    return std::make_pair(thisGridConfig, thisPathfinderStratList);
}
