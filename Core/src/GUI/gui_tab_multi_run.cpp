#include "GUI/gui_tab_multi_run.h"

#include <QStandardItemModel>
#include <QHeaderView>

GUI::MultiRunTab::MultiRunTab(QWidget *parent) :
        QWidget(parent),
        configTable(new QTableView(this)),
        itemModel(new QStandardItemModel(this)),
        mainLayout(new QVBoxLayout(this)),
        configForm(new MultiConfigForm(this)),
        configTableLayout(new QHBoxLayout()),
        buttonLayout(new QVBoxLayout()),
        addConfigButton(new QPushButton("+", this)),
        removeConfigButton(new QPushButton("-", this)),
        startButton(new QPushButton("Start", this)) {
    configTable->setModel(itemModel);

    configForm->disable();
    configTable->horizontalHeader()->setVisible(false);
    configTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    configTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    configTable->horizontalHeader()->setSectionsClickable(false);

    setupConnections();

    buttonLayout->addWidget(addConfigButton);
    buttonLayout->addWidget(removeConfigButton);

    configTableLayout->addWidget(configTable);
    configTableLayout->addLayout(buttonLayout);

    mainLayout->addLayout(configTableLayout);
    mainLayout->addWidget(configForm);
    mainLayout->addWidget(startButton);

    setLayout(mainLayout);


}

void GUI::MultiRunTab::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (!selected.indexes().isEmpty()) {
        const auto &index = selected.indexes().at(0);
    }
}

void GUI::MultiRunTab::setupConnections() {
    connect(configTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &MultiRunTab::onSelectionChanged);

    connect(addConfigButton,
            &QPushButton::clicked,
            this, &MultiRunTab::addOrSaveConfiguration);

    connect(removeConfigButton,
            &QPushButton::clicked,
            this, &MultiRunTab::removeSelectedConfiguration);

}

void GUI::MultiRunTab::addOrSaveConfiguration() {
    if (addConfigButton->text() == "+") {
        addConfigButton->setText("Save");
        addConfigButton->setStyleSheet("background-color: green;");
        configForm->enable();
        configForm->resetForm();
        auto *dummyItem = new QStandardItem("Waiting for Input...");
        itemModel->appendRow(dummyItem);
        dummyRowIndex = itemModel->rowCount() -1;
    } else {
        addConfigButton->setStyleSheet("");
        addConfigButton->setText("+");
        configForm->disable();
        auto params = configForm->getFormParams();
        configForm->resetForm();
        auto *item = new QStandardItem();
        QString gridWidthHeightString = QString("%1 x %2")
                .arg(params.first.gridHeight)
                .arg(params.first.gridWith);
        QString obstacleGenString = QString::fromStdString(
                GridGenerator::ObstacleGenStrategyParser::obstacleGenStrategyToDisplayableText.at(
                        params.first.obstacleGenStrategy));
        QString obstacleDensityString = QString("OD: %1%").arg(params.first.obstacleDensity * 100.0);
        QString minStartEndDistanceString = QString("SE: %1%").arg(params.first.minStartEndDistance * 100.0);

        QString pathfinderString = "[";
        for (auto iter = params.second.begin();
             iter != params.second.end();
             iter++) {
            if (iter != params.second.begin()) {
                pathfinderString += ", ";
            }
            if (Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.find(*iter) !=
                Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.end()) {
                pathfinderString += QString::fromStdString(
                        Pathfinder::PathfinderStrategyParser::pathfindingStrategyToDisplayableText.at(*iter));
            }
        }
        pathfinderString += "]";

        QString itemText = QString("%1, %2, %3, %4, %5")
                .arg(gridWidthHeightString,
                     obstacleGenString,
                     obstacleDensityString,
                     minStartEndDistanceString,
                     pathfinderString);
        item->setText(itemText);
        itemModel->setItem(dummyRowIndex, item);
        configTable->clearSelection();
    }
}

void GUI::MultiRunTab::removeSelectedConfiguration() {
    QItemSelectionModel *selected = configTable->selectionModel();
    if (selected->hasSelection()) {
        itemModel->removeRow(selected->selectedRows().first().row());
    }
}
