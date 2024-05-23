#include "../inc/pathfinding.h"

void Pathfinder::DijkstraSolve::markShortestPath() {
    GridGenerator::Cell* start = grid.getStart();
    GridGenerator::Cell* end = grid.getEnd();

    std::map<GridGenerator::Cell*, uint8_t> distances;


}