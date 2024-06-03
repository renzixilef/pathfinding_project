#pragma once

#include <QWidget>

#include "GridGenerator/grid.h"


namespace GUI {

    class GridDrawerWidget : public QWidget {
    Q_OBJECT
    public:
        explicit GridDrawerWidget(const GridGenerator::Grid &grid, QWidget *parent = nullptr) :
                QWidget(parent), grid(grid) {}

    private:
        const GridGenerator::Grid &grid;
        void paintEvent(QPaintEvent*) override;
    };

}
