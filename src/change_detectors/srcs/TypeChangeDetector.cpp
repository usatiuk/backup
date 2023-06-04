//
// Created by Stepan Usatiuk on 12.05.2023.
//

#include "../includes/TypeChangeDetector.h"

bool TypeChangeDetector::check(const ComparableFile &f1, const ComparableFile &f2) const {
    return f1.type != f2.type;
}
