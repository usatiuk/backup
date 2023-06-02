//
// Created by Stepan Usatiuk on 12.05.2023.
//

#ifndef SEMBACKUP_RUNNINGDIFFAVERAGE_H
#define SEMBACKUP_RUNNINGDIFFAVERAGE_H

#include <functional>

#include "RunningAverage.h"

/// Computes the rolling average of differences between last sampled and currently sampled numbers
class RunningDiffAverage {
public:
    ///
    /// \param getFunc Function that samples the value
    /// \param max     Max number of samples to average
    /// \param ms      Sampling period
    RunningDiffAverage(std::function<unsigned long long()> getFunc, int max, int ms);

    /// Returns the average
    unsigned long long get();

private:
    unsigned long long prev = 0;  ///< Previously sampled value
    RunningAverage runningAverage;///< Backing RunningAverage
};


#endif//SEMBACKUP_RUNNINGDIFFAVERAGE_H
