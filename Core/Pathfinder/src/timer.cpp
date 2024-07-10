#include "timer.h"

void Pathfinder::PathfinderTimer::stepBegin() {
    stepStartTime = std::chrono::high_resolution_clock::now();
}

void Pathfinder::PathfinderTimer::stepEnd() {
    stepCount++;
    auto stepEndTime = std::chrono::high_resolution_clock::now();
    auto stepDuration = std::chrono::duration_cast<std::chrono::microseconds>(
            stepEndTime - stepStartTime).count();
    absoluteUSecondsTime += static_cast<double>(stepDuration);
    avgUSecondsPerStep = static_cast<double>(absoluteUSecondsTime) /stepCount;
}
