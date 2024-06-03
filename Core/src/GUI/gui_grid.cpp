#include "GUI/gui_grid.h"

#include <QPainter>

void GUI::GridDrawerWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    uint32_t cellSide = std::floor(std::min(double(width()) / grid.getSizeX(),double(height()) / grid.getSizeY()));

    const auto startCoord = grid.getStartCoordinates();
    const auto endCoord = grid.getEndCoordinates();

    QPen linePen(Qt::black);
    linePen.setWidth(std::floor(width()/1000));
    painter.setPen(linePen);
    painter.setRenderHint(QPainter::Antialiasing, false);

    for (uint32_t x = 0; x < grid.getSizeX(); x++) {
        for (uint32_t y = 0; y < grid.getSizeY(); y++) {
            GridGenerator::GridCoordinate cellCoord{x, y};
            const GridGenerator::Cell &cell = grid(cellCoord);
            QColor cellColor;
            if(startCoord == cellCoord || endCoord == cellCoord){
               cellColor = GridGenerator::CellStateParser::parseCellStateToQColor(cell.getState(), true);
            }else{
                cellColor = GridGenerator::CellStateParser::parseCellStateToQColor(cell.getState());
            }
            QRectF cellRect(x*cellSide, y*cellSide, cellSide, cellSide);
            painter.fillRect(cellRect, cellColor);
            painter.drawRect(cellRect);

        }
    }
}
