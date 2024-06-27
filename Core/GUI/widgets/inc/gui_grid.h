#pragma once

#include <QWidget>
#include <QQueue>

#include "grid.h"

namespace GUI::Widgets {

    class GridDrawerWidget : public QWidget {
    Q_OBJECT
    public:
        explicit GridDrawerWidget(const GridGenerator::Grid &grid, QWidget *parent = nullptr) :
                QWidget(parent), grid(grid) {}

        inline void resetPixmapQueue() {pixmapQueue.clear();}

        void exportPixmapQueue();

        inline void toggleStartEndRedefinitionPhase() { startEndRedefinitionEnabled = !startEndRedefinitionEnabled; }

        [[nodiscard]] inline bool getRedefinitionStatus() const { return startEndRedefinitionEnabled; }

    private:

        void paintEvent(QPaintEvent *) override;

        void mousePressEvent(QMouseEvent *event) override;

        const GridGenerator::Grid &grid;
        QQueue<QPixmap> pixmapQueue;
        uint32_t cellSide = 0;
        bool startEndRedefinitionEnabled = false;
    };

}
