#pragma once

#include "grid.h"
#include <vector>
#include <map>

namespace Pathfinder {

    class pathfindingParent {
    public:
        explicit pathfindingParent(GridGenerator::Grid& grid) : grid(grid){}
        virtual void markShortestPath() = 0;
        virtual inline std::map<GridGenerator::Cell*, GridGenerator::Cell*> getShortestPath(){return shortestPath;};

    protected:
        std::map<GridGenerator::Cell*, GridGenerator::Cell*> shortestPath;
        GridGenerator::Grid& grid;

    };

    class DijkstraSolve : public pathfindingParent{
    public:
        void markShortestPath() override;
    };

}