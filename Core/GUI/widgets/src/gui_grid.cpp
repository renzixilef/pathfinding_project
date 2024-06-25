#include "gui_grid.h"

#include <QPainter>
#include <QMouseEvent>

void GUI::Widgets::GridDrawerWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    cellSide = std::floor(std::min(double(width()) / grid.getSizeX(), double(height()) / grid.getSizeY()));

    const auto startCoord = grid.getStartCoordinates();
    const auto endCoord = grid.getEndCoordinates();

    QPen linePen(Qt::black);
    linePen.setWidth(std::floor(width() / 1000));
    painter.setPen(linePen);
    painter.setRenderHint(QPainter::Antialiasing, false);

    for (uint32_t x = 0; x < grid.getSizeX(); x++) {
        for (uint32_t y = 0; y < grid.getSizeY(); y++) {
            GridGenerator::GridCoordinate cellCoord{x, y};
            const GridGenerator::Cell &cell = grid(cellCoord);
            QColor cellColor;
            if (startCoord == cellCoord || endCoord == cellCoord) {
                cellColor = GridGenerator::CellStateParser::parseCellStateToQColor(cell.getState(), true);
            } else {
                cellColor = GridGenerator::CellStateParser::parseCellStateToQColor(cell.getState());
            }
            QRectF cellRect(x * cellSide, y * cellSide, cellSide, cellSide);
            painter.fillRect(cellRect, cellColor);
            painter.drawRect(cellRect);

        }
    }
}

void GUI::Widgets::GridDrawerWidget::mousePressEvent(QMouseEvent *event) {
    if (startEndRedefinitionEnabled &&
        (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)) {
        // Get grid cell clicked
        int x = event->pos().x();
        int y = event->pos().y();

        int cellX = std::floor(static_cast<double>(x) / cellSide);
        int cellY = std::floor(double(y) / cellSide);

        GridGenerator::GridCoordinate cellCoord{static_cast<uint32_t>(cellX),
                                                static_cast<uint32_t>(cellY)};
        if (grid(cellCoord).getState() != GridGenerator::CellState::CELL_OBSTACLE) {
            if (event->button() == Qt::LeftButton) const_cast<GridGenerator::Grid &>(grid).setStart(cellCoord);
            if (event->button() == Qt::RightButton) const_cast<GridGenerator::Grid &>(grid).setEnd(cellCoord);
            update();
        }
    }
}
