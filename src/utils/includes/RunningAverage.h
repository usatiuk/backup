//
// Created by Stepan Usatiuk on 05.05.2023.
//

#ifndef SEMBACKUP_RUNNINGAVERAGE_H
#define SEMBACKUP_RUNNINGAVERAGE_H

#include <atomic>
#include <deque>
#include <functional>
#include <mutex>
#include <numeric>
#include <thread>

/// Class to compute running average of some value
class RunningAverage {
public:
    ///
    /// \param getFunc Function that samples the value
    /// \param max     Max number of samples to average
    /// \param ms      Sampling period
    RunningAverage(std::function<unsigned long long()> getFunc, int max, int ms);

    /// Destructor, instructs the thread to exit
    ~RunningAverage();

    /// Returns the average
    unsigned long long get();

private:
    std::atomic<bool> stop = false;             ///< Stop signal
    std::function<unsigned long long()> getFunc;///< Sampling function
    std::deque<unsigned long long> data;        ///< Data collected
    int max;                                    ///< Max number of samples
    int ms;                                     ///< Sampling period
    std::mutex dataLock;                        ///< Deque lock
    std::thread thread;                         ///< Worker thread

    /// Worker thread loop
    void loop();
};


#endif//SEMBACKUP_RUNNINGAVERAGE_H
