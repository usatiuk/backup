//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "../includes/Cleaner.h"

Cleaner::Cleaner(std::vector<std::filesystem::path> toClean) : toClean(std::move(toClean)) {
    for (const auto &p: this->toClean) {
        std::filesystem::remove_all(p);
    }
}

Cleaner::~Cleaner() {
    for (const auto &p: toClean) {
        std::filesystem::remove_all(p);
    }
}
