#pragma once

#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QStandardItem>

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
        QLineEdit *configLine;
        QVBoxLayout *mainLayout;


        QPushButton *startButton;
        QPushButton *addConfigButton;
        QPushButton *removeConfigButton;

    };
}