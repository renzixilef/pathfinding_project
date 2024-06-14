#include "GUI/gui_dialog_multi_run.h"

#include <QPushButton>

GUI::MultiRunDialog::MultiRunDialog(std::queue<std::pair<RunInterface::RunGridConfig,
        std::list<Pathfinder::PathfinderStrategy>>> queue,
                                    QWidget *parent) :

        QDialog(parent),
        runQueue(std::move(queue)),
        multiRunThread(new QThread(this)),
        playPauseButton(new QPushButton("Play", this)),
        mainLayout(new QVBoxLayout(this)),
        buttonLayout(new QHBoxLayout()) {


}
