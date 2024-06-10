#include "GUI/gui_tab_multi_run.h"

#include <QStandardItemModel>

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
        configForm->enable();
        configForm->resetForm();
    } else {
        addConfigButton->setText("+");
        configForm->disable();
        auto params = configForm->getFormParams();
        configForm->resetForm();
        auto *item = new QStandardItem();
        QString obstacleGenText = QString::fromStdString(
                GridGenerator::ObstacleGenStrategyParser::obstacleGenStrategyToDisplayableText.at(
                        params.first.obstacleGenStrategy));
        QString itemText = QString("%1").arg(obstacleGenText);
        item->setText(itemText);
        itemModel->appendRow(item);
        configTable->clearSelection();
    }
}

void GUI::MultiRunTab::removeSelectedConfiguration() {
    QItemSelectionModel *selected = configTable->selectionModel();
    if (selected->hasSelection()) {
        itemModel->removeRow(selected->selectedRows().first().row());
    }
}
