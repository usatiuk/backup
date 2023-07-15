//
// Created by Stepan Usatiuk on 05.05.2023.
//

#include "RunningAverage.h"

RunningAverage::RunningAverage(std::function<unsigned long long int()> getFunc, int max, int ms)
    : getFunc(std::move(getFunc)), max(max), ms(ms), thread(&RunningAverage::loop, this) {
}

void RunningAverage::loop() {
    while (!stop) {
        {
            std::lock_guard lock(dataLock);
            data.emplace_front(getFunc());
            if (data.size() > max) data.pop_back();
        }
        std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(ms)));
    }
}

RunningAverage::~RunningAverage() {
    stop = true;
    thread.join();
}

unsigned long long RunningAverage::get() {
    std::lock_guard lock(dataLock);
    if (data.empty()) return 0;
    return std::accumulate(data.begin(), data.end(), 0UL) / data.size();
}
