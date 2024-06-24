#include "cell.h"

QColor GridGenerator::CellStateParser::parseCellStateToQColor(GridGenerator::CellState state, bool &&startEnd) {
    if (startEnd) return {Qt::green};
    switch (state) {
        case CellState::CELL_OBSTACLE:
            return {Qt::black};
        case CellState::CELL_CLOSED:
            return {Qt::red};
        default:
        case CellState::CELL_OPEN:
            return {Qt::white};
        case CellState::CELL_PATH:
            return {Qt::blue};
        case CellState::CELL_VISITED:
            return {Qt::yellow};
    }
}
