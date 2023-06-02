//
// Created by Stepan Usatiuk on 23.04.2023.
//

#ifndef SEMBACKUP_FILTERAES_H
#define SEMBACKUP_FILTERAES_H

#include <array>
#include <cstdint>
#include <string>

#include "Filter.h"

/// Filter implementation that encrypts/decrypts data using provided password and salt
class FilterAES : public Filter {
public:
    /// Constructs the filter, using \p password and \p salt to generate the encryption key
    /// \param password Constant reference to password string
    /// \param salt     Constant reference to salt string
    FilterAES(const std::string &password, const std::string &salt);

    /// Encrypts the \p from vector
    /// \copydoc Filter::filterWrite
    /// \throws  Exception on any error
    std::vector<char> filterWrite(std::vector<char> from) const override;

    /// Decrypts the \p from vector
    /// \copydoc Filter::filterRead
    /// \throws  Exception on any error
    std::vector<char> filterRead(std::vector<char> from) const override;

private:
    const std::array<uint8_t, 32> key;///< Key used for encryption, derived from \p password and \p salt
};


#endif//SEMBACKUP_FILTERAES_H
