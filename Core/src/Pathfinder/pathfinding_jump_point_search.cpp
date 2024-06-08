#include "Pathfinder/pathfinding.h"

#include <iostream>
#include <queue>

void Pathfinder::JumpPointSolve::nextStep() {
    using GridGenerator::GridCoordinate;
    GridCoordinate endCoordinates = grid.getEndCoordinates();
    GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();

    grid(currentCoordinates).markClosed();
    grid.incrementClosedCellCount();

    for (const auto &direction: GridGenerator::Grid::offsets) {
        std::optional<GridCoordinate> jumpPoint = jump(currentCoordinates, direction);
        if (jumpPoint != std::nullopt) {
            GridCoordinate jumpPointCoord = jumpPoint.value();
            GridGenerator::Cell &jumpPointCell = grid(jumpPointCoord);
            double gCostFromCurrent = (grid(currentCoordinates).getCost().gCost +
                                       currentCoordinates.getOctileDistanceTo(jumpPointCoord));
            if (jumpPointCoord == endCoordinates) {
                jumpPointCell.setParentDirPair(std::make_pair(-direction.first, -direction.second));
                jumpPointCell.setGCost(gCostFromCurrent);
                jumpPointCell.setHCost(jumpPointCoord.getAbsDistanceTo(endCoordinates));
                grid.markPathByParentCells(false);
                return;
            }
            if (jumpPointCell.getState() == GridGenerator::CellState::CELL_CLOSED)
                continue;
            if (jumpPointCell.getState() == GridGenerator::CellState::CELL_VISITED) {
                if (jumpPointCell.getCost().gCost > gCostFromCurrent) {
                    nextCellQueue.push(jumpPointCoord);
                    jumpPointCell.setParentDirPair(std::make_pair(-direction.first, -direction.second));
                    jumpPointCell.setGCost(gCostFromCurrent);
                    grid.incrementVisitedCellCount();
                }
                continue;
            }
            if (jumpPointCell.getState() == GridGenerator::CellState::CELL_OPEN) {
                nextCellQueue.push(jumpPointCoord);
                jumpPointCell.setParentDirPair(std::make_pair(-direction.first, -direction.second));
                jumpPointCell.setGCost(gCostFromCurrent);
                jumpPointCell.setHCost(jumpPointCoord.getAbsDistanceTo(endCoordinates));
                jumpPointCell.markVisited();
                grid.incrementVisitedCellCount();
            }
        }
    }
}

std::optional<GridGenerator::GridCoordinate> Pathfinder::JumpPointSolve::jump(
        GridGenerator::GridCoordinate currentCoord, std::pair<int8_t, int8_t> direction) {
    using GridGenerator::GridCoordinate;

    if (isCellBlockedOrOutOfBounds(currentCoord.x + direction.first, currentCoord.y + direction.second)) {
        return std::nullopt;
    }
    GridCoordinate nextPos(currentCoord.x + direction.first,
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
            nextPos = {nextPos.x + direction.first, nextPos.y + direction.second};
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
                nextPos = {nextPos.x + direction.first, nextPos.y + direction.second};
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
                nextPos = {nextPos.x + direction.first, nextPos.y + direction.second};
                if (nextPos == grid.getEndCoordinates()) {
                    return nextPos;
                }
            }
        }
    }
    return std::nullopt;
}

void Pathfinder::JumpPointSolve::initJPSSolver() {
    GridGenerator::GridCoordinate startCoord = grid.getStartCoordinates();
    directionMap[startCoord].insert(directionMap[startCoord].end(),
                                    GridGenerator::Grid::offsets.begin(),
                                    GridGenerator::Grid::offsets.end());
}
