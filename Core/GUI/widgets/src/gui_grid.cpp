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

    QFont fontSmall;
    fontSmall.setPointSize(4);
    fontSmall.setFamily("Arial");

    QFont fontLarge;
    fontLarge.setPointSize(6);
    fontLarge.setFamily("Arial");

    double margin = cellSide * 0.1;

    for (uint32_t x = 0; x < grid.getSizeX(); x++) {
        for (uint32_t y = 0; y < grid.getSizeY(); y++) {
            GridGenerator::GridCoordinate cellCoord{x, y};
            const GridGenerator::Cell &cell = grid(cellCoord);
            GridGenerator::CellState cellState = cell.getState();
            QColor cellColor;
            if (startCoord == cellCoord || endCoord == cellCoord) {
                cellColor = GridGenerator::CellStateParser::parseCellStateToQColor(cellState, true);
            } else {
                cellColor = GridGenerator::CellStateParser::parseCellStateToQColor(cellState);
            }
            QRectF cellRect(x * cellSide, y * cellSide, cellSide, cellSide);
            painter.fillRect(cellRect, cellColor);
            painter.drawRect(cellRect);

            if (cellState == GridGenerator::CellState::CELL_VISITED ||
                cellState == GridGenerator::CellState::CELL_PATH ||
                cellState == GridGenerator::CellState::CELL_CLOSED) {
                QString cellRectGCost = QString("%1").arg(QString::number(cell.getCost().gCost, 'f', 1));
                QString cellRectHCost = QString("%1").arg(QString::number(cell.getCost().hCost, 'f', 1));
                QString cellRectTotalCost = QString("%1").arg(QString::number(cell.getCost().totalCost(), 'f', 1));
                QRectF gCostRect = cellRect.adjusted(margin, margin, -margin, -static_cast<float>(cellSide) / 2);
                QRectF hCostRect = cellRect.adjusted(static_cast<float>(cellSide) / 2, margin,
                                                     -margin, -static_cast<float>(cellSide) / 2);
                QRectF totalCostRect = cellRect.adjusted(margin, static_cast<float>(cellSide) / 2, -margin,
                                                         -margin);
                if (cellSide >= 25) {
                    painter.setFont(fontSmall);
                    painter.drawText(gCostRect, Qt::AlignLeft | Qt::AlignTop, cellRectGCost);
                    painter.drawText(hCostRect, Qt::AlignRight | Qt::AlignTop, cellRectHCost);
                }
                if (cellSide >= 15) {
                    painter.setFont(fontLarge);
                    painter.drawText(totalCostRect, Qt::AlignCenter, cellRectTotalCost);
                }


            }
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
