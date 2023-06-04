//
// Created by Stepan Usatiuk on 16.04.2023.
//

#include "../includes/EditTimeChangeDetector.h"

bool EditTimeChangeDetector::check(const ComparableFile &f1, const ComparableFile &f2) const {
    return f1.mtime != f2.mtime;
}
