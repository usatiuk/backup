//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_FILEREPOSITORYTEST_H
#define SEMBACKUP_FILEREPOSITORYTEST_H


#include <fstream>
#include <iostream>
#include <vector>

#include "../../src/repo/FileRepository.h"
#include "../../src/repo/objects/Chunk.h"
#include "../utils/Cleaner.h"
#include "../utils/TestGroupGenerator.h"

class FileRepositoryTest : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_FILEREPOSITORYTEST_H
