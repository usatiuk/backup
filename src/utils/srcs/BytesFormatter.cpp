//
// Created by Stepan Usatiuk on 13.05.2023.
//

#include "BytesFormatter.h"

#include <iomanip>
#include <sstream>

BytesFormatter::BytesFormat BytesFormatter::format(unsigned long long int bytes) {
    std::stringstream outNum;
    outNum << std::fixed << std::setprecision(2);

    if (bytes > 1024UL * 1024 * 1024 * 1024) {
        outNum << (double) bytes / (1024.0 * 1024.0 * 1024.0 * 1024.0);
        return {outNum.str(), "TiB"};
    }
    if (bytes > 1024UL * 1024 * 1024) {
        outNum << (double) bytes / (1024.0 * 1024.0 * 1024.0);
        return {outNum.str(), "GiB"};
    }
    if (bytes > 1024UL * 1024) {
        outNum << (double) bytes / (1024.0 * 1024.0);
        return {outNum.str(), "MiB"};
    }
    if (bytes > 1024UL) {
        outNum << (double) bytes / (1024.0);
        return {outNum.str(), "KiB"};
    }
    outNum << bytes;
    return {outNum.str(), "Bytes"};
}

std::string BytesFormatter::formatStr(unsigned long long int bytes) {
    auto fmt = format(bytes);
    return fmt.number + " " + fmt.prefix;
}