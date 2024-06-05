#include "Pathfinder/pathfinding.h"

#include <iostream>
#include <queue>

void Pathfinder::JumpPointSolve::nextStep() {
    GridGenerator::GridCoordinate endCoordinates = grid.getEndCoordinates();
    GridGenerator::GridCoordinate currentCoordinates = nextCellQueue.top();
    nextCellQueue.pop();

    GridGenerator::Cell &currentCell = grid(currentCoordinates);
    if (currentCell.getState() == GridGenerator::CellState::CELL_CLOSED) return;
    auto nextDist = directionMap[currentCoordinates].back();
    directionMap[currentCoordinates].pop_back();
    addJumpPointsToQueue(currentCoordinates, nextDist, currentCoordinates, false);
}

void Pathfinder::JumpPointSolve::addJumpPointsToQueue(GridGenerator::GridCoordinate currentCoord,
                                                      std::pair<int8_t, int8_t> direction,
                                                      GridGenerator::GridCoordinate basePoint,
                                                      bool recursed) {
    using GridGenerator::GridCoordinate;
    // TODO: CALCULATE G AND H COST SOMEWHERE?
    std::vector<std::pair<int8_t, int8_t>> insertionDirs;

    if (!(0 <= currentCoord.x + direction.first < grid.getSizeX() &&
          0 <= currentCoord.y + direction.second < grid.getSizeY())) {
        if (!recursed) grid(currentCoord).markClosed();
        return;
    }
    GridCoordinate nextPos(currentCoord.x + direction.first,
                           currentCoord.y + direction.second);
    if (grid(nextPos).getState() == GridGenerator::CellState::CELL_OBSTACLE) {
        if (!recursed) grid(currentCoord).markClosed();
        grid.markPathByParentCells();
        return;
    }
    grid(nextPos).setGCost(grid(currentCoord).getCost().gCost +
                           currentCoord.getAbsDistanceTo(nextPos));
    if (nextPos == grid.getEndCoordinates()) {
        grid(nextPos).setParent(&grid(basePoint));
        return;
    }
    // diagonal move
    if (std::abs(direction.first) == std::abs(direction.second)) {
        addJumpPointsToQueue(nextPos, std::make_pair(direction.first, 0),
                             basePoint, true);
        addJumpPointsToQueue(nextPos, std::make_pair(0, direction.second),
                             basePoint, true);
        addJumpPointsToQueue(nextPos, direction, basePoint, true);
        if (!recursed) grid(currentCoord).markClosed();
    } else { //horizontal/vertical move
        if (0 <= currentCoord.x + 2 * direction.first < grid.getSizeX() &&
            0 <= currentCoord.y + 2 * direction.second < grid.getSizeY()) {
            GridCoordinate nextNextPos(currentCoord.x + 2 * direction.first,
                                       currentCoord.y + 2 * direction.second);
            GridCoordinate nextFirstPerp{nextPos.x + direction.second,
                                         nextPos.y + direction.first};
            GridCoordinate nextNextFirstPerp{nextNextPos.x + direction.second,
                                             nextNextPos.y + direction.first};
            std::pair<int8_t, int8_t> firstPerpDir = GridCoordinate::getDirection(nextPos,
                                                                                  nextNextFirstPerp);
            GridCoordinate nextSecondPerp{nextPos.x - direction.second,
                                          nextPos.y - direction.first};
            GridCoordinate nextNextSecondPerp{nextNextPos.x - direction.second,
                                              nextNextPos.y - direction.first};
            std::pair<int8_t, int8_t> secondPerpDir = GridCoordinate::getDirection(nextPos,
                                                                                   nextNextSecondPerp);
            std::pair<int8_t, int8_t> secondPerpendicularDir{};
            if ((grid(nextFirstPerp).getState() == GridGenerator::CellState::CELL_OBSTACLE) &&
                (grid(nextNextFirstPerp).getState() != GridGenerator::CellState::CELL_OBSTACLE)) {
                insertionDirs.push_back(firstPerpDir);
            }
            if ((grid(nextSecondPerp).getState() == GridGenerator::CellState::CELL_OBSTACLE) &&
                (grid(nextNextSecondPerp).getState() != GridGenerator::CellState::CELL_OBSTACLE)) {
                insertionDirs.push_back(secondPerpDir);
            }
        }
        if (insertionDirs.size() >= 0) {
            insertionDirs.insert(insertionDirs.begin(), direction);
            grid(nextPos).setHCost(grid.getEndCoordinates().getAbsDistanceTo(nextPos));
            grid(nextPos).setParent(&grid(basePoint));
            nextCellQueue.push(nextPos);
            directionMap[nextPos].insert(directionMap[nextPos].end(),
                                         insertionDirs.begin(),
                                         insertionDirs.end());
            return;
        } else {
            addJumpPointsToQueue(nextPos, direction, basePoint, true);
            if (!recursed) grid(currentCoord).markClosed();
        }
    }

}

void Pathfinder::JumpPointSolve::initJPSSolver() {
    std::vector<std::pair<int8_t, int8_t>> offsets = {
            {-1, 0},
            {1,  0},
            {0,  -1},
            {0,  1},  // Top, Bottom, Left, Right
            {-1, -1},
            {-1, 1},
            {1,  -1},
            {1,  1}  // Diagonals
    };
    GridGenerator::GridCoordinate startCoord = grid.getStartCoordinates();
    directionMap[startCoord].insert(directionMap[startCoord].end(), offsets.begin(), offsets.end());
}
