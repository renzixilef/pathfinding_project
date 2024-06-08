#pragma once

#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>

namespace GUI{
    class MultiRunTab : public QWidget{
        Q_OBJECT
    public:
        explicit MultiRunTab(QWidget *parent = nullptr);

    private:
        QTableWidget *configTable;
        QLineEdit *configLine;
        QVBoxLayout *mainLayout;
    };
}