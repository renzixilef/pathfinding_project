#include "GUI/gui_main_window.h"
#include "GUI/gui_tab_single_run.h"

GUI::MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), tabWidget(new QTabWidget(this)) {
    setCentralWidget(tabWidget);

    auto* tabSingleRun = new GUI::SingleRunTab();
    tabWidget->addTab(tabSingleRun, "Single Run");

    //tabWidget->currentWidget()->update();

    //auto* tabStartBenchmark = new QWidget();
    //tabWidget->addTab(tabStartBenchmark, "Start Benchmark");

}
