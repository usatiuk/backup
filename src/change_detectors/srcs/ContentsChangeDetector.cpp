//
// Created by Stepan Usatiuk on 05.05.2023.
//

#include "ContentsChangeDetector.h"

#include <iterator>

bool ContentsChangeDetector::check(const ComparableFile &f1, const ComparableFile &f2) const {
    if (f1.type != f2.type) return true;

    auto b1 = f1.contents();
    auto b2 = f2.contents();

    return !std::equal(std::istreambuf_iterator<char>(b1.get()), std::istreambuf_iterator<char>(),
                       std::istreambuf_iterator<char>(b2.get()), std::istreambuf_iterator<char>());
}
