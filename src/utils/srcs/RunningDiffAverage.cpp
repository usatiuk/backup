//
// Created by Stepan Usatiuk on 12.05.2023.
//

#include "../includes/RunningDiffAverage.h"

RunningDiffAverage::RunningDiffAverage(std::function<unsigned long long int()> getFunc, int max, int ms)
    : runningAverage(
              [this, get = std::move(getFunc)] {
                  auto cur = get();
                  auto calc = cur - prev;
                  prev = cur;
                  return calc;
              },
              max, ms) {
}

unsigned long long RunningDiffAverage::get() {
    return runningAverage.get();
}
