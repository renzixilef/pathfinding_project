#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

namespace GUI {
    class SingleRunDialog : public QDialog {
        Q_OBJECT

    public:
        SingleRunDialog(QWidget *parent = nullptr);

    };
}