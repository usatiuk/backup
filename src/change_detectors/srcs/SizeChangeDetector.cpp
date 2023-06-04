//
// Created by Stepan Usatiuk on 16.04.2023.
//

#include "../includes/SizeChangeDetector.h"

bool SizeChangeDetector::check(const ComparableFile &f1, const ComparableFile &f2) const {
    return f1.bytes != f2.bytes;
}
