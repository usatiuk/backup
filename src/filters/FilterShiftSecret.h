//
// Created by Stepan Usatiuk on 23.04.2023.
//
#ifdef TEST
#ifndef SEMBACKUP_FILTERSHIFTSECRET_H
#define SEMBACKUP_FILTERSHIFTSECRET_H

#include <string>

#include "Filter.h"

/// Filter implementation that shifts every byte in input vector using two provided value
/// \warning For testing purposes only!
class FilterShiftSecret : public Filter {
public:
    /// Constructs the filter using the sum of first bytes of \p password and \p salt to initialize shiftVal
    /// \param password Constant reference to "password" string
    /// \param salt     Constant reference to "salt" string
    FilterShiftSecret(const std::string &password, const std::string &salt);

    /// \copydoc Filter::filterWrite
    std::vector<char> filterWrite(std::vector<char> from) const override;

    /// \copydoc Filter::filterRead
    std::vector<char> filterRead(std::vector<char> from) const override;

private:
    int shiftVal = 0;///< Value to add to input bytes
};


#endif//SEMBACKUP_FILTERSHIFTSECRET_H
#endif//TEST