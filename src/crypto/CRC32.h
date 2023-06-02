//
// Created by Stepan Usatiuk on 12.05.2023.
//

#ifndef SEMBACKUP_CRC32_H
#define SEMBACKUP_CRC32_H

#include <cstdint>
#include <vector>

#include <zlib.h>

/// Utility class to compute CRC32 values of vectors of chars
class CRC32 {
public:
    using crcType = uLong;

    /// Calculates the CRC32 of given vector
    /// \param in Constant reference to a vector of chars
    /// \return   CRC32 result
    static crcType calculate(const std::vector<char> &in);
};


#endif//SEMBACKUP_CRC32_H
