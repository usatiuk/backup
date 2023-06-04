//
// Created by Stepan Usatiuk on 22.04.2023.
//

#include "../includes/FilterContainer.h"

FilterContainer::FilterContainer() = default;

FilterContainer &FilterContainer::addFilter(std::unique_ptr<Filter> &&f) {
    filters.emplace_back(std::move(f));
    return *this;
}

std::vector<char> FilterContainer::filterWrite(std::vector<char> from) const {
    for (auto const &f: filters) from = f->filterWrite(std::move(from));
    return from;
}

std::vector<char> FilterContainer::filterRead(std::vector<char> from) const {
    for (auto f = filters.crbegin(); f != filters.crend(); f++)
        from = (*f)->filterRead(std::move(from));
    return from;
}
