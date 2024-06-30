#pragma once

#include <QWidget>
#include <QQueue>
#include <opencv4/opencv2/opencv.hpp>

#include "grid.h"

namespace GUI::Widgets {

    class GridDrawerWidget : public QWidget {
    Q_OBJECT
    public:
        explicit GridDrawerWidget(const GridGenerator::Grid &grid, QWidget *parent = nullptr) :
                QWidget(parent), grid(grid) {}

        inline void enqueueNextPixmap() { pixmapQueue.enqueue(this->grab()); }

        inline void resetPixmapQueue() { pixmapQueue.clear(); }

        void exportPixmapQueue(const std::string &filename) const;

        inline void toggleStartEndRedefinitionPhase() { startEndRedefinitionEnabled = !startEndRedefinitionEnabled; }

        [[nodiscard]] inline bool getRedefinitionStatus() const { return startEndRedefinitionEnabled; }

    private:

        void paintEvent(QPaintEvent *) override;

        void mousePressEvent(QMouseEvent *event) override;

        static cv::Mat pixmapToMat(const QPixmap &pixmap, int32_t width, int32_t height);

        const GridGenerator::Grid &grid;
        QQueue<QPixmap> pixmapQueue;
        uint32_t cellSide = 0;
        bool startEndRedefinitionEnabled = false;
    };

}
