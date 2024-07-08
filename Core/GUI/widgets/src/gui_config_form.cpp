#include "gui_config_form.h"
#include "grid.h"
#include "obstacle_gen.h"

#include <unordered_set>

GUI::Widgets::ConfigFormParent::ConfigFormParent(QWidget *parent) : QWidget(parent),
                                                                    gridHeightSpinBox(new QSpinBox(this)),
                                                                    gridWidthSpinBox(new QSpinBox(this)),
                                                                    obstacleDensitySpinBox(new QDoubleSpinBox(this)),
                                                                    minStartEndDistanceSpinBox(
                                                                            new QDoubleSpinBox(this)),
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

void GUI::Widgets::ConfigFormParent::resetForm() {
    gridHeightSpinBox->setValue(20);
    gridWidthSpinBox->setValue(20);
    obstacleDensitySpinBox->setValue(0.0);
    minStartEndDistanceSpinBox->setValue(0.0);

    gridGeneratorAlgorithmComboBox->setCurrentIndex(0);
}

void GUI::Widgets::ConfigFormParent::disable() {
    gridHeightSpinBox->setDisabled(true);
    gridWidthSpinBox->setDisabled(true);
    obstacleDensitySpinBox->setDisabled(true);
    minStartEndDistanceSpinBox->setDisabled(true);
    gridGeneratorAlgorithmComboBox->setDisabled(true);
}

void GUI::Widgets::ConfigFormParent::enable() {
    gridHeightSpinBox->setEnabled(true);
    gridWidthSpinBox->setEnabled(true);
    obstacleDensitySpinBox->setEnabled(true);
    minStartEndDistanceSpinBox->setEnabled(true);
    gridGeneratorAlgorithmComboBox->setEnabled(true);
}

GUI::Widgets::SingleConfigForm::SingleConfigForm(QWidget *parent) : ConfigFormParent(parent),
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

std::variant<RunInterface::MultiRunConfig, RunInterface::SingleRunConfig>
GUI::Widgets::SingleConfigForm::getFormParams() {
    RunInterface::RunGridConfig thisGridConfig = {
            static_cast<uint32_t>(gridWidthSpinBox->value()),
            static_cast<uint32_t>(gridHeightSpinBox->value()),
            static_cast<float>(obstacleDensitySpinBox->value()),
            static_cast<float>(minStartEndDistanceSpinBox->value()),
            static_cast<GridGenerator::ObstacleGenStrategy>(
                    gridGeneratorAlgorithmComboBox->currentData().toUInt())
    };

    auto thisStrat = static_cast<Pathfinder::PathfinderStrategy>(pathfindingAlgorithmComboBox->currentData().toUInt());
    return RunInterface::SingleRunConfig(thisGridConfig, thisStrat);

}

void GUI::Widgets::SingleConfigForm::resetForm() {
    ConfigFormParent::resetForm();
    pathfindingAlgorithmComboBox->setCurrentIndex(0);
}

void GUI::Widgets::SingleConfigForm::enable() {
    ConfigFormParent::enable();
    pathfindingAlgorithmComboBox->setEnabled(true);
}

void GUI::Widgets::SingleConfigForm::disable() {
    ConfigFormParent::disable();
    pathfindingAlgorithmComboBox->setDisabled(true);
}

GUI::Widgets::MultiConfigForm::MultiConfigForm(QWidget *parent) : ConfigFormParent(parent),
                                                                  pathfindingAlgorithmListWidget(new QListWidget(this)),
                                                                  iterationsSpinBox(new QSpinBox(this)),
                                                                  invalidInputWarningLabel(new QLabel(this)),
                                                                  repeatUnsolvablesCheckBox(new QCheckBox(this)) {
    using Pathfinder::PathfinderStrategyParser;
    invalidInputWarningLabel->hide();
    invalidInputWarningLabel->setStyleSheet("QLabel {color: red;}");
    invalidInputWarningLabel->setText("Select at least one pathfinding strategy!");
    gridHeightSpinBox->setRange(20, 1000);
    gridWidthSpinBox->setRange(20, 1000);
    obstacleDensitySpinBox->setRange(0.0, 0.7);
    obstacleDensitySpinBox->setSingleStep(0.05);
    minStartEndDistanceSpinBox->setRange(0.0, 0.9);
    minStartEndDistanceSpinBox->setSingleStep(0.05);
    iterationsSpinBox->setRange(1, 100000);

    for (const auto &[k, v]:
            PathfinderStrategyParser::pathfindingStrategyToDisplayableText) {
        auto *item = new QListWidgetItem(QString::fromStdString(v));
        item->setData(Qt::UserRole, static_cast<uint8_t>(k));
        pathfindingAlgorithmListWidget->addItem(item);
    }

    pathfindingAlgorithmListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    layout->addRow("Iterations", iterationsSpinBox);
    layout->addRow("Pathfinding Algorithm", pathfindingAlgorithmListWidget);
    layout->addRow("Repeat Unsolvables", repeatUnsolvablesCheckBox);
    layout->addRow(invalidInputWarningLabel);
    setLayout(layout);
}

void GUI::Widgets::MultiConfigForm::enable() {
    ConfigFormParent::enable();
    pathfindingAlgorithmListWidget->setEnabled(true);
    iterationsSpinBox->setEnabled(true);
}

void GUI::Widgets::MultiConfigForm::disable() {
    ConfigFormParent::disable();
    pathfindingAlgorithmListWidget->setDisabled(true);
    iterationsSpinBox->setDisabled(true);
    invalidInputWarningLabel->hide();
}

void GUI::Widgets::MultiConfigForm::resetForm() {
    ConfigFormParent::resetForm();
    pathfindingAlgorithmListWidget->clearSelection();
    iterationsSpinBox->setValue(1);
}

std::variant<RunInterface::MultiRunConfig, RunInterface::SingleRunConfig>
GUI::Widgets::MultiConfigForm::getFormParams() {
    RunInterface::RunGridConfig thisGridConfig = {
            static_cast<uint32_t>(gridWidthSpinBox->value()),
            static_cast<uint32_t>(gridHeightSpinBox->value()),
            static_cast<float>(obstacleDensitySpinBox->value()),
            static_cast<float>(minStartEndDistanceSpinBox->value()),
            static_cast<GridGenerator::ObstacleGenStrategy>(
                    gridGeneratorAlgorithmComboBox->currentData().toUInt()),
            iterationsSpinBox->value(),
            repeatUnsolvablesCheckBox->isChecked()
    };

    QList<QListWidgetItem *> selectedItems = pathfindingAlgorithmListWidget->selectedItems();
    std::list<Pathfinder::PathfinderStrategy> thisPathfinderStratList;
    for (auto *item: selectedItems) {
        thisPathfinderStratList.push_back(
                static_cast<Pathfinder::PathfinderStrategy>(item->data(Qt::UserRole).toUInt()));
    }
    return RunInterface::MultiRunConfig(thisGridConfig, thisPathfinderStratList);
}

void
GUI::Widgets::MultiConfigForm::populate(const RunInterface::MultiRunConfig &config) {
    gridHeightSpinBox->setValue(static_cast<int32_t>(config.gridConfig.gridHeight));
    gridWidthSpinBox->setValue(static_cast<int32_t>(config.gridConfig.gridWidth));
    obstacleDensitySpinBox->setValue(config.gridConfig.obstacleDensity);
    minStartEndDistanceSpinBox->setValue(config.gridConfig.minStartEndDistance);
    gridGeneratorAlgorithmComboBox->setCurrentIndex(static_cast<uint8_t>(config.gridConfig.obstacleGenStrategy) - 1);
    if (config.gridConfig.repeatUnsolvables.has_value())
        repeatUnsolvablesCheckBox->setChecked(config.gridConfig.repeatUnsolvables.value());
    else
        repeatUnsolvablesCheckBox->setChecked(false);

    if (config.gridConfig.iterations.has_value())
        iterationsSpinBox->setValue(static_cast<int32_t>(config.gridConfig.iterations.value()));
    else
        iterationsSpinBox->setValue(1);


    QItemSelectionModel *selectionModel = pathfindingAlgorithmListWidget->selectionModel();
    std::unordered_set<uint8_t> stratSet;
    for (const auto &strat: config.strats) {
        stratSet.insert(static_cast<uint8_t>(strat));
    }
    for (int32_t i = 0; i < pathfindingAlgorithmListWidget->count(); i++) {
        QListWidgetItem *item = pathfindingAlgorithmListWidget->item(i);
        uint8_t itemValue = item->data(Qt::UserRole).toUInt();
        if (stratSet.count(itemValue)) {
            selectionModel->select(pathfindingAlgorithmListWidget->model()->index(i, 0),
                                   QItemSelectionModel::Select);
        }
    }

}

bool GUI::Widgets::MultiConfigForm::inputValid() {
    if (pathfindingAlgorithmListWidget->selectedItems().isEmpty()) return false;
    else return true;
}

void GUI::Widgets::MultiConfigForm::handleInvalidInput() {
    invalidInputWarningLabel->setVisible(true);
}
