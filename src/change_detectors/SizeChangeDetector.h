//
// Created by Stepan Usatiuk on 16.04.2023.
//

#ifndef SEMBACKUP_SIZECHANGEDETECTOR_H
#define SEMBACKUP_SIZECHANGEDETECTOR_H

#include "ChangeDetector.h"

/// A ChangeDetector implementation that compares two files by their size
class SizeChangeDetector : public ChangeDetector {
public:
    /// \copydoc ChangeDetector::check
    /// \return ComparableFile%s are considered different if their sizes are different
    bool check(const ComparableFile &f1, const ComparableFile &f2) const override;
};


#endif//SEMBACKUP_SIZECHANGEDETECTOR_H
