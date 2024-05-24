#include "../inc/grid.h"

#include <functional>


std::vector<std::reference_wrapper<GridGenerator::Cell>> GridGenerator::Grid::getNeighbors(size_t row, size_t col) {
        std::vector<std::reference_wrapper<Cell>> neighbors;

        std::vector<std::pair<int, int>> offsets = {
                {-1, 0}, {1, 0}, {0, -1}, {0, 1},  // Top, Bottom, Left, Right
                 {-1, -1}, {-1, 1}, {1, -1}, {1, 1}  // Diagonals
        };

        for (const auto& offset : offsets) {
            int64_t neighborRow = row + offset.first;
            int64_t neighborCol = col + offset.second;
            if (neighborRow >= 0 && neighborRow < sizeX && neighborCol >= 0 && neighborCol < sizeY) {
                neighbors.push_back(std::ref(cells[neighborRow][neighborCol]));
            }
        }
        return neighbors;
}
