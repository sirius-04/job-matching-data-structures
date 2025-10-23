#ifndef PERFORMANCE_TRACKER_HPP
#define PERFORMANCE_TRACKER_HPP

#include "../../utils/MemoryUsageGetter/MemoryUsageGetter.hpp"

#include <iostream>
#include <chrono>
#include <functional>
#include <string>

using namespace std;
using namespace std::chrono;

struct PerformanceResult
{
    double timeTakenMs;
    size_t memoryUsedBytes;

    PerformanceResult(double time = 0, size_t mem = 0)
        : timeTakenMs(time), memoryUsedBytes(mem) {}

    void printPerformance(const string &label = "Performance Result") const
    {
        cout << "==== " << label << " ====\n";
        cout << "Time Taken   : " << timeTakenMs << " ms\n";
        cout << "Memory Used  : " << memoryUsedBytes << " KB\n";
        cout << "==========================\n";
    }
};

class PerformanceTracker
{
public:
    static PerformanceResult measure(const std::function<void()> &func)
    {
        size_t startMem = getCurrentMemoryUsage();
        auto start = high_resolution_clock::now();

        // --- Run algorithm ---
        func();

        auto end = high_resolution_clock::now();
        size_t endMem = getCurrentMemoryUsage();

        double elapsedMs = duration_cast<microseconds>(end - start).count() / 1000.0;
        size_t usedMem = (endMem > startMem) ? (endMem - startMem) : 0;

        return PerformanceResult(elapsedMs, usedMem);
    }
};

#endif
