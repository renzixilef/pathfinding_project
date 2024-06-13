#include "GUI/gui_dialog_multi_run.h"

#include <QPushButton>

GUI::MultiRunDialog::MultiRunDialog(const RunInterface::RunGridConfig& config,
                                    const std::list<Pathfinder::PathfinderStrategy>& strats,
                                    QWidget *parent) :

        QDialog(parent),
        runInterface(new RunInterface::MultiRun(config, strats)),
        multiRunThread(new QThread(this)),
        playPauseButton(new QPushButton("Play", this)),
        mainLayout(new QVBoxLayout(this)),
        buttonLayout(new QHBoxLayout()) {

}
