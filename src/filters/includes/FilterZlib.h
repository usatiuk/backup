//
// Created by Stepan Usatiuk on 23.04.2023.
//

#ifndef SEMBACKUP_FILTERZLIB_H
#define SEMBACKUP_FILTERZLIB_H

#include "Filter.h"

/// Filter implementation that uses Zlib to compress data
class FilterZlib : public Filter {
public:
    /// Creates the filter using \p level as compression level
    /// \param level
    FilterZlib(int level);

    /// Compresses the \p from vector
    /// \copydoc Filter::filterWrite
    /// \throws  Exception on any error
    std::vector<char> filterWrite(std::vector<char> from) const override;

    /// Decompresses the \p from vector
    /// \copydoc Filter::filterRead
    /// \throws  Exception on any error
    std::vector<char> filterRead(std::vector<char> from) const override;

private:
    int level = -1;///< Compression level to use, -1 is the Zlib default
};

#endif//SEMBACKUP_FILTERZLIB_H
