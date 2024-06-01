#pragma once

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>

#include "../RunInterface/runner.h"

namespace GUI {
    class SingleRunDialog : public QDialog {
    Q_OBJECT

    public:
        SingleRunDialog(RunInterface::RunGridConfig config,
                        Pathfinder::PathfinderStrategy strat,
                        QWidget *parent = nullptr);

    private:
        RunInterface::SingleRun runInterface;


    };
}