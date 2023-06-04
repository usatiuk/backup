//
// Created by Stepan Usatiuk on 04.05.2023.
//

#include "../includes/ChangeDetectorContainer.h"

#include <functional>

bool ChangeDetectorContainer::check(const ComparableFile &f1, const ComparableFile &f2) const {
    return std::any_of(changeDetectors.begin(), changeDetectors.end(),
                       [&](const auto &changeDetector) {
                           return changeDetector->check(f1, f2);
                       });
}

ChangeDetectorContainer::ChangeDetectorContainer(std::vector<std::unique_ptr<ChangeDetector>> &&changeDetectors) : changeDetectors(std::move(changeDetectors)) {}
