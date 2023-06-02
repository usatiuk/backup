//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_CLEANER_H
#define SEMBACKUP_CLEANER_H


#include <filesystem>
#include <utility>
#include <vector>

class Cleaner {
public:
    Cleaner(std::vector<std::filesystem::path> toClean);
    ~Cleaner();

private:
    std::vector<std::filesystem::path> toClean;
};


#endif//SEMBACKUP_CLEANER_H
