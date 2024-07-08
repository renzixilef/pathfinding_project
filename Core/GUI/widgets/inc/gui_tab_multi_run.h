#pragma once

#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QStandardItem>

#include "gui_config_form.h"
#include "headless_multi_run.h"

namespace GUI::Widgets {
    class MultiRunItem : public QStandardItem {
    public:
        explicit MultiRunItem(RunInterface::MultiRunConfig itemConfig);

        inline RunInterface::MultiRunConfig getConfig(){return itemConfig;}
        inline void setTextBasedOnParams(){
            setText(Application::HeadlessRunner::generateConfigQString(itemConfig));
        }
    private:
        RunInterface::MultiRunConfig itemConfig;
    };


    class MultiRunTab : public QWidget {
    Q_OBJECT
    public:
        explicit MultiRunTab(QWidget *parent = nullptr);

    private slots:

        void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

        void addOrSaveConfiguration();

        void removeSelectedConfiguration();

        void startRuns();

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

        MultiConfigForm *configForm;

        int32_t dummyRowIndex{-1};

    };
}