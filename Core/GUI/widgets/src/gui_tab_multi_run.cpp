#include "gui_tab_multi_run.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <utility>
#include "gui_dialog_multi_run.h"

GUI::Widgets::MultiRunTab::MultiRunTab(QWidget *parent) :
        QWidget(parent),
        configTable(new QTableView(this)),
        itemModel(new QStandardItemModel(this)),
        mainLayout(new QVBoxLayout(this)),
        configForm(new MultiConfigForm(this)),
        configTableLayout(new QHBoxLayout()),
        buttonLayout(new QVBoxLayout()),
        addConfigButton(new QPushButton("Add", this)),
        removeConfigButton(new QPushButton("Remove", this)),
        startButton(new QPushButton("Start", this)) {
    configTable->setModel(itemModel);
    configForm->disable();
    configTable->horizontalHeader()->setVisible(false);
    configTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    configTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    configTable->horizontalHeader()->setSectionsClickable(false);

    setupConnections();

    addConfigButton->setStyleSheet("background-color: green;");
    removeConfigButton->setEnabled(false);
    startButton->setEnabled(true);
    removeConfigButton->setStyleSheet("");

    buttonLayout->addWidget(addConfigButton);
    buttonLayout->addWidget(removeConfigButton);

    configTableLayout->addWidget(configTable);
    configTableLayout->addLayout(buttonLayout);

    mainLayout->addLayout(configTableLayout);
    mainLayout->addWidget(configForm);
    mainLayout->addWidget(startButton);

    setLayout(mainLayout);


}

void GUI::Widgets::MultiRunTab::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (!selected.indexes().isEmpty()) {
        const auto &row = selected.indexes().at(0).row();
        auto selectedItem = dynamic_cast<MultiRunItem *>(itemModel->item(row));

        addConfigButton->setText("Save");
        addConfigButton->setStyleSheet("background-color: blue;");
        startButton->setDisabled(true);
        configForm->enable();
        configForm->populate(selectedItem->getConfig());
        removeConfigButton->setEnabled(true);
        removeConfigButton->setStyleSheet("background-color: red;");
        dummyRowIndex = row;
    } else {
        addConfigButton->setText("Add");
        addConfigButton->setStyleSheet("background-color: green;");
        startButton->setEnabled(true);
        configForm->disable();
        configForm->resetForm();
        removeConfigButton->setEnabled(false);
        removeConfigButton->setStyleSheet("");
        dummyRowIndex = -1;

    }
}

void GUI::Widgets::MultiRunTab::setupConnections() {
    connect(configTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &MultiRunTab::onSelectionChanged);

    connect(addConfigButton,
            &QPushButton::clicked,
            this, &MultiRunTab::addOrSaveConfiguration);

    connect(removeConfigButton,
            &QPushButton::clicked,
            this, &MultiRunTab::removeSelectedConfiguration);

    connect(startButton,
            &QPushButton::clicked,
            this, &MultiRunTab::startRuns);

}

void GUI::Widgets::MultiRunTab::addOrSaveConfiguration() {
    if (addConfigButton->text() == "Add") {
        addConfigButton->setText("Save");
        addConfigButton->setStyleSheet("background-color: blue;");
        startButton->setDisabled(true);
        configForm->enable();
        configForm->resetForm();
        removeConfigButton->setEnabled(false);
        removeConfigButton->setStyleSheet("");

        auto *dummyItem = new QStandardItem("Waiting for Input...");
        itemModel->appendRow(dummyItem);
        dummyRowIndex = itemModel->rowCount() - 1;
    } else {
        if(configForm->inputValid()) {
            addConfigButton->setStyleSheet("background-color: green;");
            addConfigButton->setText("Add");
            startButton->setEnabled(true);
            configForm->disable();
            auto params = configForm->getFormParams();
            configForm->resetForm();
            auto *item = new MultiRunItem(std::get<RunInterface::MultiRunConfig>(params));
            itemModel->setItem(dummyRowIndex, item);
            configTable->clearSelection();
            removeConfigButton->setEnabled(false);
            removeConfigButton->setStyleSheet("");
        }else{
            configForm->handleInvalidInput();
        }
    }
}

void GUI::Widgets::MultiRunTab::removeSelectedConfiguration() {
    QItemSelectionModel *selected = configTable->selectionModel();
    if (selected->hasSelection()) {
        itemModel->removeRow(selected->selectedRows().first().row());
    }
    configTable->clearSelection();
    removeConfigButton->setEnabled(false);
    removeConfigButton->setStyleSheet("");
    addConfigButton->setStyleSheet("background-color: green;");
    addConfigButton->setText("Add");
    startButton->setEnabled(true);
    configForm->disable();

}

void GUI::Widgets::MultiRunTab::startRuns() {
    std::queue<std::pair<RunInterface::MultiRunConfig, QString>> runQueue;
    uint32_t rows = itemModel->rowCount();
    for (uint32_t i = 0; i < rows; i++) {
        auto item = dynamic_cast<MultiRunItem *>(itemModel->item(static_cast<int>(i)));
        runQueue.emplace(item->getConfig(), item->text());
    }
    QDialog *multiRunDialog = new GUI::MultiRunDialog(runQueue, this);

    multiRunDialog->exec();
}

GUI::Widgets::MultiRunItem::MultiRunItem(RunInterface::MultiRunConfig itemConfig) :
        QStandardItem(),
        itemConfig(std::move(itemConfig)) {
    setTextBasedOnParams();
}
