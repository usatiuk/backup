//
// Created by Stepan Usatiuk on 04.05.2023.
//

#ifndef SEMBACKUP_CHANGEDETECTORCONTAINER_H
#define SEMBACKUP_CHANGEDETECTORCONTAINER_H

#include <memory>
#include <vector>

#include "ChangeDetector.h"
#include "ComparableFile.h"

/// Wrapper for multiple ChangeDetector%s
/** A ChangeDetector implementation that serves as a convenience wrapper for
 *  multiple ChangeDetector%s, its check returns true if any of the wrapped ChangeDetector%s return true
 */
class ChangeDetectorContainer : public ChangeDetector {
public:
    /// Constructs a ChangeDetectorContainer using a vector of existing ChangeDetector%s
    /// \param changeDetectors An rvalue reference to a vector of unique pointers of ChangeDetector
    ChangeDetectorContainer(std::vector<std::unique_ptr<ChangeDetector>> &&changeDetectors);

    /// \copydoc ChangeDetector::check
    /// \return ComparableFile%s are considered different if any of the wrapped ChangeDetector%s return true
    bool check(const ComparableFile &f1, const ComparableFile &f2) const override;

private:
    std::vector<std::unique_ptr<ChangeDetector>> changeDetectors;
};


#endif//SEMBACKUP_CHANGEDETECTORCONTAINER_H
