#include "pathfinding.h"

#include <queue>

void Pathfinder::JumpPointSolve::nextStep() {
    using GridGenerator::GridCoordinate;
    timer.stepBegin();
    GridCoordinate endCoordinates = grid.getEndCoordinates();
    GridCoordinate currentCoordinates = nextCellQueue.top();
    std::pair<uint32_t, uint32_t> parentDir = {0, 0};
    if (currentCoordinates != grid.getStartCoordinates()) {
        parentDir = GridCoordinate::getDirection(
                currentCoordinates, static_cast<GridCoordinate>(*grid(currentCoordinates).getParentIfCoord()));
    }
    nextCellQueue.pop();

    grid(currentCoordinates).markClosed();

    for (const auto &direction: GridGenerator::Grid::offsets) {

        if (parentDir.first == direction.first && parentDir.second == direction.second) continue;
        std::optional<GridCoordinate> jumpPoint = jump(currentCoordinates, direction);
        if (jumpPoint != std::nullopt) {
            GridCoordinate jumpPointCoord = jumpPoint.value();
            GridGenerator::Cell &jumpPointCell = grid(jumpPointCoord);
            double gCostFromCurrent = (grid(currentCoordinates).getCost().gCost +
                                       currentCoordinates.getOctileDistanceTo(jumpPointCoord));
            if (jumpPointCoord == endCoordinates) {
                jumpPointCell.setParentCoordinates(std::pair<uint32_t, uint32_t>(currentCoordinates));
                jumpPointCell.setGCost(gCostFromCurrent);
                jumpPointCell.setHCost(jumpPointCoord.getAbsDistanceTo(endCoordinates));
                timer.stepEnd();
                grid.markPathByParentCells(false);
                return;
            }
            if (jumpPointCell.getState() == GridGenerator::CellState::CELL_OPEN) {
                jumpPointCell.setParentCoordinates(std::pair<uint32_t, uint32_t>(currentCoordinates));
                jumpPointCell.setGCost(gCostFromCurrent);
                jumpPointCell.setHCost(jumpPointCoord.getAbsDistanceTo(endCoordinates));
                jumpPointCell.markVisited();
                grid.incrementVisitedCellCount();
                nextCellQueue.push(jumpPointCoord);
            }
        }
    }
    grid.incrementClosedCellCount();
    timer.stepEnd();
}

std::optional<GridGenerator::GridCoordinate> Pathfinder::JumpPointSolve::jump(
        GridGenerator::GridCoordinate currentCoord, std::pair<int8_t, int8_t> direction) {
    using GridGenerator::GridCoordinate;

    if (isCellBlockedOrOutOfBounds(currentCoord.x + direction.first, currentCoord.y + direction.second)) {
        return std::nullopt;
    }
    GridCoordinate nextPos = GridCoordinate(currentCoord.x + direction.first,
                                            currentCoord.y + direction.second);

    if (nextPos == grid.getEndCoordinates()) {
        return nextPos;
    }
    // diagonal move
    if (direction.first != 0 && direction.second != 0) {
        while (true) {
            if ((!isCellBlockedOrOutOfBounds(nextPos.x - direction.first, nextPos.y + direction.second) &&
                 isCellBlockedOrOutOfBounds(nextPos.x - direction.first, nextPos.y)) ||
                (!isCellBlockedOrOutOfBounds(nextPos.x + direction.first, nextPos.y - direction.second) &&
                 isCellBlockedOrOutOfBounds(nextPos.x, nextPos.y - direction.second))) {
                return nextPos;
            }

            if (jump(nextPos, std::make_pair(direction.first, 0)) != std::nullopt ||
                jump(nextPos, std::make_pair(0, direction.second)) != std::nullopt) {
                return nextPos;
            }

            if (isCellBlockedOrOutOfBounds(nextPos.x + direction.first, nextPos.y + direction.second)) {
                return std::nullopt;
            }
            nextPos = GridCoordinate(nextPos.x + direction.first, nextPos.y + direction.second);
            if (nextPos == grid.getEndCoordinates()) {
                return nextPos;
            }
        }
    } else {
        if (direction.first != 0) {
            while (true) {
                if ((!isCellBlockedOrOutOfBounds(nextPos.x + direction.first, nextPos.y + 1) &&
                     isCellBlockedOrOutOfBounds(nextPos.x, nextPos.y + 1)) ||
                    (!isCellBlockedOrOutOfBounds(nextPos.x + direction.first, nextPos.y - 1) &&
                     isCellBlockedOrOutOfBounds(nextPos.x, nextPos.y - 1))) {
                    return nextPos;
                }
                if (isCellBlockedOrOutOfBounds(nextPos.x + direction.first, nextPos.y + direction.second)) {
                    return std::nullopt;
                }
                nextPos = GridCoordinate(nextPos.x + direction.first, nextPos.y + direction.second);
                if (nextPos == grid.getEndCoordinates()) {
                    return nextPos;
                }
            }
        }
        if (direction.second != 0) {
            while (true) {
                if ((!isCellBlockedOrOutOfBounds(nextPos.x + 1, nextPos.y + direction.second) &&
                     isCellBlockedOrOutOfBounds(nextPos.x + 1, nextPos.y)) ||
                    (!isCellBlockedOrOutOfBounds(nextPos.x - 1, nextPos.y + direction.second) &&
                     isCellBlockedOrOutOfBounds(nextPos.x - 1, nextPos.y))) {
                    return nextPos;
                }
                if (isCellBlockedOrOutOfBounds(nextPos.x + direction.first, nextPos.y + direction.second)) {
                    return std::nullopt;
                }
                nextPos = GridCoordinate(nextPos.x + direction.first, nextPos.y + direction.second);
                if (nextPos == grid.getEndCoordinates()) {
                    return nextPos;
                }
            }
        }
    }
    return std::nullopt;
}
