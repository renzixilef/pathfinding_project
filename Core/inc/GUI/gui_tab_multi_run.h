#pragma once

#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QStandardItem>

#include "gui_config_form.h"

namespace GUI {
    class MultiRunItem : public QStandardItem {
    public:
        explicit MultiRunItem(RunInterface::RunGridConfig config,
                              std::list<Pathfinder::PathfinderStrategy> strats);

        inline RunInterface::RunGridConfig getGridConfig(){return itemConfig;}
        inline std::list<Pathfinder::PathfinderStrategy> getPathfinderList(){return itemStrats;}
        void setTextBasedOnParams();
    private:
        RunInterface::RunGridConfig itemConfig;
        std::list<Pathfinder::PathfinderStrategy> itemStrats;
    };


    class MultiRunTab : public QWidget {
    Q_OBJECT
    public:
        explicit MultiRunTab(QWidget *parent = nullptr);

    private slots:

        void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

        void addOrSaveConfiguration();

        void removeSelectedConfiguration();

    private:
        static const QRegularExpression lineDataRegex;

        void setupConnections();

        QStandardItemModel *itemModel;
        QTableView *configTable;

        QVBoxLayout *mainLayout;

        QHBoxLayout *configTableLayout;
        QVBoxLayout *buttonLayout;

        QPushButton *startButton;
        QPushButton *addConfigButton;
        QPushButton *removeConfigButton;

        MultiConfigForm *configForm;

        int dummyRowIndex{-1};

    };
}