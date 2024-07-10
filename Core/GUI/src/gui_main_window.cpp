#include "gui_main_window.h"
#include "gui_tab_multi_run.h"
#include "gui_tab_single_run.h"

GUI::MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), tabWidget(new QTabWidget(this)) {
    setCentralWidget(tabWidget);
    setWindowTitle("Pathfinder 1.0");
    auto* tabSingleRun = new GUI::Widgets::SingleRunTab();
    tabWidget->addTab(tabSingleRun, "Single Run");

    auto* tabMultiRun = new GUI::Widgets::MultiRunTab();
    tabWidget->addTab(tabMultiRun, "Multi Run/Benchmark");

}
