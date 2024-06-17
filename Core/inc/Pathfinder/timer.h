#pragma once

#include <chrono>

namespace Pathfinder {
    class PathfinderTimer {
    public:
        PathfinderTimer() = default;

        void stepBegin();

        void stepEnd();

        [[nodiscard]] inline double getAvgUSecondsPerStep() const { return avgUSecondsPerStep; }

        [[nodiscard]] inline double getAbsoluteTime() const{return absoluteUSecondsTime;}
        [[nodiscard]] inline uint32_t getStepCount() const{return stepCount;}

    private:
        double avgUSecondsPerStep = 0;
        double absoluteUSecondsTime = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> stepStartTime;
        uint32_t stepCount = 0;
    };
}

