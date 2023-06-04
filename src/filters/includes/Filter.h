//
// Created by Stepan Usatiuk on 22.04.2023.
//

#ifndef SEMBACKUP_FILTER_H
#define SEMBACKUP_FILTER_H

#include <vector>

/// Interface class for I/O filters
class Filter {
public:
    /// Applies the filter to \p from vector and returns the result
    /// Note: the vector is passed by value, as it allows to avoid copying with std::move in case the filter modifies the \p in vector in-place
    /// \param from Source vector of chars
    /// \return     Filtered vector of chars
    virtual std::vector<char> filterWrite(std::vector<char> from) const = 0;

    /// Reverses the applied filter from \p from vector and returns the result
    /// Note: the vector is passed by value, as it allows to avoid copying with std::move in case the filter modifies the \p in vector in-place
    /// \param from Source vector of chars
    /// \return     Filtered vector of chars
    virtual std::vector<char> filterRead(std::vector<char> from) const = 0;

    /// Default virtual destructor
    virtual ~Filter();
};


#endif//SEMBACKUP_FILTER_H
