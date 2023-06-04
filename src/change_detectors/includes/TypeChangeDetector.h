//
// Created by Stepan Usatiuk on 12.05.2023.
//

#ifndef SEMBACKUP_TYPECHANGEDETECTOR_H
#define SEMBACKUP_TYPECHANGEDETECTOR_H

#include "ChangeDetector.h"

/// A ChangeDetector implementation that compares two files by their type
class TypeChangeDetector : public ChangeDetector {
public:
    /// \copydoc ChangeDetector::check
    /// \return ComparableFile%s are considered different if their types are different
    bool check(const ComparableFile &f1, const ComparableFile &f2) const override;
};


#endif//SEMBACKUP_TYPECHANGEDETECTOR_H
