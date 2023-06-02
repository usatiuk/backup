//
// Created by Stepan Usatiuk on 12.05.2023.
//

#include "CRC32.h"

CRC32::crcType CRC32::calculate(const std::vector<char> &in) {
    crcType res = crc32(0L, nullptr, 0);
    res = crc32(res, reinterpret_cast<const Bytef *>(in.data()), in.size());
    return res;
}
