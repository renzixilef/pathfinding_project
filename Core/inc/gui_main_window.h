#pragma once

#include <QMainWindow>
#include <QTabWidget>

namespace GUI{
    class MainWindow : public QMainWindow{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget * parent = nullptr);
    private:
        QTabWidget* tabWidget;
    };
}