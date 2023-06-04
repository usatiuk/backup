//
// Created by Stepan Usatiuk on 22.04.2023.
//
#ifndef SEMBACKUP_FILTERSHIFT_H
#define SEMBACKUP_FILTERSHIFT_H

#include "Filter.h"

/// Filter implementation that shifts every byte in input vector using provided value
/// \warning For testing purposes only!
class FilterShift : public Filter {
public:
    /// Constructs the filter using \p level as shift value
    /// \param level Number that will be added to each input byte
    FilterShift(int level);

    /// \copydoc Filter::filterWrite
    std::vector<char> filterWrite(std::vector<char> from) const override;

    /// \copydoc Filter::filterRead
    std::vector<char> filterRead(std::vector<char> from) const override;

private:
    int shiftVal;///< Value to add to input bytes
};


#endif//SEMBACKUP_FILTERSHIFT_H
