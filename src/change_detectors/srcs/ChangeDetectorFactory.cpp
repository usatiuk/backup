//
// Created by Stepan Usatiuk on 16.04.2023.
//

#include "../includes/ChangeDetectorFactory.h"

#include <vector>

#include "../includes/ContentsChangeDetector.h"
#include "../includes/EditTimeChangeDetector.h"
#include "../includes/SizeChangeDetector.h"
#include "../includes/TypeChangeDetector.h"
#include "Exception.h"

std::unique_ptr<ChangeDetector> ChangeDetectorFactory::getChangeDetector(const std::string &type) {
    if (type == "etime") {
        return std::make_unique<EditTimeChangeDetector>();
    } else if (type == "size") {
        return std::make_unique<SizeChangeDetector>();
    } else if (type == "type") {
        return std::make_unique<TypeChangeDetector>();
    } else if (type == "contents") {
        return std::make_unique<ContentsChangeDetector>();
    } else
        throw Exception("Unknown ChangeDetector type " + type);
}

ChangeDetectorContainer ChangeDetectorFactory::getChangeDetectors(const Config &config) {
    std::vector<std::unique_ptr<ChangeDetector>> changeDetectors;
    for (auto const &i: config.getList("change-detectors")) {
        changeDetectors.emplace_back(ChangeDetectorFactory::getChangeDetector(i));
    }

    return ChangeDetectorContainer(std::move(changeDetectors));
}
