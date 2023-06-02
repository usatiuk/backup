//
// Created by Stepan Usatiuk on 16.04.2023.
//

#ifndef SEMBACKUP_CHANGEDETECTORFACTORY_H
#define SEMBACKUP_CHANGEDETECTORFACTORY_H

#include <memory>
#include <string>

#include "../Config.h"
#include "ChangeDetector.h"
#include "ChangeDetectorContainer.h"

/// Factory class for ChangeDetector
/** Can create either a vector of ChangeDetector%s according to Config,
 *  or an individual ChangeDetector from a type string
 */
class ChangeDetectorFactory {
public:
    /// Creates a ChangeDetector of given type and returns an unique pointer to it
    /// \param type Constant reference to a string containing type of the ChangeDetector to create
    /// \return     Unique pointer to constructed ChangeDetector
    static std::unique_ptr<ChangeDetector> getChangeDetector(const std::string &type);

    /// Constructs a vector of unique pointers to ChangeDetector%s according to the given \p config
    /// \param config Config with comma-separated "change-detectors" option set, for each entry a ChangeDetector will be created
    /// \return A vector of unique pointers to ChangeDetector%s constructed according to \p config
    static ChangeDetectorContainer getChangeDetectors(const Config &config);
};


#endif//SEMBACKUP_CHANGEDETECTORFACTORY_H
