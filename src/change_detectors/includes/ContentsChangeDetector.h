//
// Created by Stepan Usatiuk on 05.05.2023.
//

#ifndef SEMBACKUP_CONTENTSCHANGEDETECTOR_H
#define SEMBACKUP_CONTENTSCHANGEDETECTOR_H

#include "ChangeDetector.h"

/// A ChangeDetector implementation that compares two files by their contents
class ContentsChangeDetector : public ChangeDetector {
public:
    /// \copydoc ChangeDetector::check
    /// \return ComparableFile%s are considered different if their contents are different
    bool check(const ComparableFile &f1, const ComparableFile &f2) const override;
};


#endif//SEMBACKUP_CONTENTSCHANGEDETECTOR_H
