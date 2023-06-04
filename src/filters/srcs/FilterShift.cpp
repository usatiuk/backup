//
// Created by Stepan Usatiuk on 22.04.2023.
//
#include "../includes/FilterShift.h"

std::vector<char> FilterShift::filterWrite(std::vector<char> from) const {
    for (auto &c: from) c += shiftVal;
    return from;
}

std::vector<char> FilterShift::filterRead(std::vector<char> from) const {
    for (auto &c: from) c -= shiftVal;
    return from;
}

FilterShift::FilterShift(int level) : shiftVal(level) {}
