//
// Created by Stepan Usatiuk on 16.04.2023.
//

#ifndef SEMBACKUP_EDITTIMECHANGEDETECTOR_H
#define SEMBACKUP_EDITTIMECHANGEDETECTOR_H


#include "ChangeDetector.h"

/// A ChangeDetector implementation that compares two files by their modification time
class EditTimeChangeDetector : public ChangeDetector {
public:
    /// \copydoc ChangeDetector::check
    /// \return ComparableFile%s are considered different if their modification times are different
    bool check(const ComparableFile &f1, const ComparableFile &f2) const override;
};


#endif//SEMBACKUP_EDITTIMECHANGEDETECTOR_H
