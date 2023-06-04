//
// Created by Stepan Usatiuk on 22.04.2023.
//

#ifndef SEMBACKUP_FILTERCONTAINER_H
#define SEMBACKUP_FILTERCONTAINER_H

#include <memory>
#include <vector>

#include "Filter.h"

/// Convenience Filter implementation, that applies multiple Filter%s in succession
class FilterContainer : public Filter {
public:
    /// Constructs an empty FilterContainer
    FilterContainer();

    /// Adds a Filter into itself
    /// \param f Rvalue reference to a unique pointer to Filter
    /// \return  Reference to itself
    FilterContainer &addFilter(std::unique_ptr<Filter> &&f);

    /// Applies the filters in order of insertion
    /// \copydoc Filter::filterWrite
    std::vector<char> filterWrite(std::vector<char> from) const override;

    /// Applies the filters in reverse order of insertion
    /// \copydoc Filter::filterRead
    std::vector<char> filterRead(std::vector<char> from) const override;

private:
    std::vector<std::unique_ptr<Filter>> filters;///< Vector of unique pointers to Filter%s
};


#endif//SEMBACKUP_FILTERCONTAINER_H
