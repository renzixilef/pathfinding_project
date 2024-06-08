#include "GUI/gui_main_window.h"
#include "GUI/gui_tab_single_run.h"
#include "GUI/gui_tab_multi_run.h"

GUI::MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), tabWidget(new QTabWidget(this)) {
    setCentralWidget(tabWidget);

    auto* tabSingleRun = new GUI::SingleRunTab();
    tabWidget->addTab(tabSingleRun, "Single Run");

    auto* tabMultiRun = new GUI::MultiRunTab();
    tabWidget->addTab(tabMultiRun, "Multi Run/Benchmark");

}
