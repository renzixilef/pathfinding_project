#include "GUI/gui_main_window.h"

GUI::MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), tabWidget(new QTabWidget(this)) {
    setCentralWidget(tabWidget);

    auto* tabSingleRun = new QWidget();
    tabWidget->addTab(tabSingleRun, "Single Run");

    auto* tabStartBenchmark = new QWidget();
    tabWidget->addTab(tabStartBenchmark, "Start Benchmark");

}
