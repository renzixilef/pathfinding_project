#include "gui_tab_multi_run.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <utility>
#include <QFileDialog>
#include <QMessageBox>
#include "gui_dialog_multi_run.h"
#include "command_line_parser.h"

GUI::Widgets::MultiRunTab::MultiRunTab(QWidget *parent) :
        QWidget(parent),
        configTable(new QTableView(this)),
        itemModel(new QStandardItemModel(this)),
        mainLayout(new QVBoxLayout(this)),
        configForm(new MultiConfigForm(this)),
        configTableLayout(new QHBoxLayout()),
        buttonLayout(new QVBoxLayout()),
        addConfigButton(new QPushButton("Add")),
        removeConfigButton(new QPushButton("Remove")),
        startButton(new QPushButton("Start")),
        importJsonButton(new QPushButton("Import JSON")){
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
    buttonLayout->addWidget(importJsonButton);

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
        importJsonButton->setDisabled(true);
        startButton->setDisabled(true);
        configForm->enable();
        configForm->populate(selectedItem->getConfig());
        removeConfigButton->setEnabled(true);
        removeConfigButton->setStyleSheet("background-color: red;");
        dummyRowIndex = row;
    } else {
        addConfigButton->setText("Add");
        addConfigButton->setStyleSheet("background-color: green;");
        importJsonButton->setEnabled(true);
        startButton->setEnabled(true);
        configForm->disable();
        configForm->resetForm();
        removeConfigButton->setDisabled(true);
        removeConfigButton->setStyleSheet("");
        dummyRowIndex = -1;

    }
}

void GUI::Widgets::MultiRunTab::setupConnections() {
    connect(configTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MultiRunTab::onSelectionChanged);

    connect(addConfigButton, &QPushButton::clicked,
            this, &MultiRunTab::addOrSaveConfiguration);

    connect(removeConfigButton, &QPushButton::clicked,
            this, &MultiRunTab::removeSelectedConfiguration);

    connect(startButton, &QPushButton::clicked,
            this, &MultiRunTab::startRuns);

    connect(importJsonButton, &QPushButton::clicked,
            this, &MultiRunTab::importJson);

}

void GUI::Widgets::MultiRunTab::addOrSaveConfiguration() {
    if (addConfigButton->text() == "Add") {
        addConfigButton->setText("Save");
        addConfigButton->setStyleSheet("background-color: blue;");
        startButton->setDisabled(true);
        configForm->enable();
        configForm->resetForm();
        removeConfigButton->setDisabled(true);
        removeConfigButton->setStyleSheet("");
        importJsonButton->setDisabled(true);

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
            removeConfigButton->setDisabled(true);
            removeConfigButton->setStyleSheet("");
            importJsonButton->setEnabled(true);
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
    removeConfigButton->setDisabled(true);
    removeConfigButton->setStyleSheet("");
    addConfigButton->setStyleSheet("background-color: green;");
    addConfigButton->setText("Add");
    importJsonButton->setEnabled(true);
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

void GUI::Widgets::MultiRunTab::importJson() {

    addConfigButton->setDisabled(true);
    startButton->setDisabled(true);
    QString filename = QFileDialog::getOpenFileName(this, tr("Import JSON config"),
                                                    "/home", tr("json Files (*.json);; All Files(*)"));

    if(!filename.isEmpty()){
        auto parsedInput = Application::PathfindingCommandParser::getInstance().parseJSONConfig(filename);
        if (std::holds_alternative<QString>(parsedInput)) {
            QMessageBox::warning(this, "Warning", std::get<QString>(parsedInput));
        }else{
            for(const auto& config:std::get<std::list<RunInterface::MultiRunConfig>>(parsedInput)){
                auto *item = new MultiRunItem(config);
                itemModel->appendRow(item);
            }
            configTable->clearSelection();
        }
    }
    addConfigButton->setEnabled(true);
    startButton->setEnabled(true);

}

GUI::Widgets::MultiRunItem::MultiRunItem(RunInterface::MultiRunConfig itemConfig) :
        QStandardItem(),
        itemConfig(std::move(itemConfig)) {
    setTextBasedOnParams();
}
