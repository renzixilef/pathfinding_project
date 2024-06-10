#include "GUI/gui_tab_multi_run.h"

#include <QStandardItemModel>

GUI::MultiRunTab::MultiRunTab(QWidget *parent) :
        QWidget(parent),
        configTable(new QTableView(this)),
    itemModel(new QStandardItemModel(this)),
        mainLayout(new QVBoxLayout(this)),
        configForm(new MultiConfigForm(this)){
    configTable->setModel(itemModel);

    setupConnections();


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
    if(addConfigButton->text() == "+"){
        addConfigButton->setText("Save");
        //formWidget.setEnabled()
        //formWidget.clear()
    }else{
        addConfigButton->setText("+");
        //formWidget.setDisabled();
        //formWidget.getParams();
        //formWidget.clear();
        //model->appendRow(configRow);
        configTable->clearSelection();
    }
}

void GUI::MultiRunTab::removeSelectedConfiguration() {
    QItemSelectionModel *selected = configTable->selectionModel();
    if(selected->hasSelection()){
        itemModel->removeRow(selected->selectedRows().first().row());
    }
}
