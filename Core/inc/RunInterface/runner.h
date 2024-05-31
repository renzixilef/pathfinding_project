#pragma once
#include <cstdint>
#include "../GridGenerator/obstacle_gen.h"

namespace RunInterface{

    struct RunGridConfig{
        uint32_t gridWith;
        uint32_t gridHeight;
        float obstacleDensity;
        float minStartEndDistance;
        GridGenerator::ObstacleGenStrategy obstacleGenStrategy;

    };


    class RunnerParent{
    public:
        explicit RunnerParent(const RunGridConfig& thisConfig);
        virtual void start()=0;
    protected:
        RunGridConfig config;
        GridGenerator::Grid grid;

    };
}