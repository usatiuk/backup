//
// Created by Stepan Usatiuk on 23.04.2023.
//

#include "../includes/FilterShiftSecret.h"

#include <string>

std::vector<char> FilterShiftSecret::filterWrite(std::vector<char> from) const {
    for (auto &c: from) c += shiftVal;
    return from;
}

std::vector<char> FilterShiftSecret::filterRead(std::vector<char> from) const {
    for (auto &c: from) c -= shiftVal;
    return from;
}

FilterShiftSecret::FilterShiftSecret(const std::string &password, const std::string &salt) {
    shiftVal = password[0] + salt[0];
}
