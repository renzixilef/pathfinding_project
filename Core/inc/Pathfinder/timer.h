/**
 * @file timer.h
 * @brief This file contains the timer class that implements pathfinding efficiency evaluation.
 */

#pragma once

#include <chrono>

/**
 * @namespace Pathfinder
 * @brief Namespace for classes and functionalities related to pathfinding algorithms.
 */
namespace Pathfinder {
    /**
     * @class PathfinderTimer
     * @brief Class used for timing and performance statistics in pathfinding algorithms.
     */
    class PathfinderTimer {
    public:
        /**
         * @brief Default constructor for PathfinderTimer class.
         */
        PathfinderTimer() = default;

        /**
         * @fn stepBegin
         * @brief Begins timing a pathfinding step.
         */
        void stepBegin();

        /**
         * @fn stepEnd
         * @brief Ends timing a pathfinding step and updates time variables.
         */
        void stepEnd();

        /**
         * @fn getAvgUSecondsPerStep
         * @brief Getter for the average microseconds per step.
         * @return Returns the average microseconds per pathfinding step.
         */
        [[nodiscard]] inline double getAvgUSecondsPerStep() const { return avgUSecondsPerStep; }

        /**
         * @fn getAbsoluteTime
         * @brief Getter for the total absolute time in microseconds.
         * @return Returns the total absolute time in microseconds.
         */
        [[nodiscard]] inline double getAbsoluteTime() const{return absoluteUSecondsTime;}

        /**
         * @fn getStepCount
         * @brief Getter for the step count.
         * @return Returns the total number of steps taken.
         */
        [[nodiscard]] inline uint32_t getStepCount() const{return stepCount;}

    private:
        /**< Stores the average micro seconds per step */
        double avgUSecondsPerStep = 0;
        /**< Stores the absolute microseconds taken */
        double absoluteUSecondsTime = 0;
        /**< Stores the start time of the current Step */
        std::chrono::time_point<std::chrono::high_resolution_clock> stepStartTime;
        /**< Stores the total number of steps taken */
        uint32_t stepCount = 0;
    };
}

