#pragma once

#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QStandardItem>

#include "gui_config_form.h"

namespace GUI {
    class MultiRunTab : public QWidget {
    Q_OBJECT
    public:
        explicit MultiRunTab(QWidget *parent = nullptr);

    private slots:

        void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        void addOrSaveConfiguration();
        void removeSelectedConfiguration();

    private:

        void setupConnections();

        QStandardItemModel *itemModel;
        QTableView *configTable;

        QVBoxLayout *mainLayout;

        QHBoxLayout *configTableLayout;
        QVBoxLayout *buttonLayout;

        QPushButton *startButton;
        QPushButton *addConfigButton;
        QPushButton *removeConfigButton;

        MultiConfigForm * configForm;

    };
}