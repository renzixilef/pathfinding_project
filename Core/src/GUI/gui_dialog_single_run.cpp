#include "GUI/gui_dialog_single_run.h"

GUI::SingleRunDialog::SingleRunDialog(RunInterface::RunGridConfig config,
                                      Pathfinder::PathfinderStrategy strat,
                                      QWidget *parent) :
        runInterface(RunInterface::SingleRun(config, strat)) {
runInterface.start();
}
