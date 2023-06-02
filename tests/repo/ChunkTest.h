//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_CHUNKTEST_H
#define SEMBACKUP_CHUNKTEST_H

#include <fstream>
#include <iostream>
#include <vector>

#include "../../src/repo/objects/Chunk.h"
#include "../utils/TestGroupGenerator.h"

class ChunkTest : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_CHUNKTEST_H
