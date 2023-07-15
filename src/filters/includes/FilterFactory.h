//
// Created by Stepan Usatiuk on 22.04.2023.
//

#ifndef SEMBACKUP_FILTERFACTORY_H
#define SEMBACKUP_FILTERFACTORY_H

#include <memory>
#include <string>

#include "Config.h"
#include "Filter.h"

/// Utility factory class for Filter%s
class FilterFactory {
public:
    /// Constructs a Filter of type \p type according to \p config
    /// \param type    Constant reference to a string containing the type of filter to construct
    /// \param config  Constant reference to Config which will be used to determine constructed Filter%'s parameters
    /// \return        Unique pointer to the constructed Filter
    static std::unique_ptr<Filter> makeFilter(const std::string &type, const Config &config);
};


#endif//SEMBACKUP_FILTERFACTORY_H
