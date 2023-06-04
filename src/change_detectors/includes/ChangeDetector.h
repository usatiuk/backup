//
// Created by Stepan Usatiuk on 16.04.2023.
//

#ifndef SEMBACKUP_CHANGEDETECTOR_H
#define SEMBACKUP_CHANGEDETECTOR_H

#include "ComparableFile.h"

/// An interface for a class comparing any two given ComparableFile%s
class ChangeDetector {
public:
    /// Abstract method for comparing two ComparableFile%s
    /// \param f1 Constant reference to the first ComparableFile
    /// \param f2 Constant reference to the second ComparableFile
    /// \return True if these objects are considered *different*, False otherwise
    virtual bool check(const ComparableFile &f1, const ComparableFile &f2) const = 0;

    /// Default virtual destructor
    virtual ~ChangeDetector();
};


#endif//SEMBACKUP_CHANGEDETECTOR_H
