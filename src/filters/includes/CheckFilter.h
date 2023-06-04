//
// Created by Stepan Usatiuk on 12.05.2023.
//

#ifndef SEMBACKUP_CHECKFILTER_H
#define SEMBACKUP_CHECKFILTER_H

#include "Filter.h"

/// Filter implementation that checks the input for corruption using CRC
/**
 *  Additionally, it has static methods for work outside FilterContainer%s
 */
class CheckFilter : public Filter {
public:
    /// \copydoc Filter::filterWrite
    /// \copydoc CheckFilter::filterWriteS
    std::vector<char> filterWrite(std::vector<char> from) const override;

    /// \copydoc Filter::filterRead
    /// \copydoc CheckFilter::filterReadS
    std::vector<char> filterRead(std::vector<char> from) const override;

    /// Adds CRC hash and magic string to the the \p from vector
    static std::vector<char> filterWriteStatic(std::vector<char> from);

    /// Checks the \p from vector and removes the metadata
    /// \throws  Exception on any error
    static std::vector<char> filterReadStatic(std::vector<char> from);

private:
    static const inline std::vector<char> magic{'s', 'e', 'm', 'b', 'a'};
};


#endif//SEMBACKUP_CHECKFILTER_H
