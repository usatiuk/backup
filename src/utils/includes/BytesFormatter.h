//
// Created by Stepan Usatiuk on 13.05.2023.
//

#ifndef SEMBACKUP_BYTESFORMATTER_H
#define SEMBACKUP_BYTESFORMATTER_H

#include <string>

/// Utility class to format byte values according to their magnitude
class BytesFormatter {
public:
    /// Structure for returning the processed byte value
    struct BytesFormat {
        std::string number;///< Number part of the value
        std::string prefix;///< Unit of measure
    };

    /// Formats the bytes in BytesFormat format
    /// \param bytes Number of bytes
    /// \return      BytesFormat value
    static BytesFormat format(unsigned long long bytes);

    /// Formats the bytes into a string
    /// \param bytes Number of bytes
    /// \return      String, consisting of the scaled number and the unit of measure separated by a space
    static std::string formatStr(unsigned long long bytes);
};


#endif//SEMBACKUP_BYTESFORMATTER_H
