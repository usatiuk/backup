//
// Created by Stepan Usatiuk on 16.04.2023.
//

#ifndef SEMBACKUP_FULLTEST_H
#define SEMBACKUP_FULLTEST_H


#include "../utils/TestGroupGenerator.h"

class FullTest : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_FULLTEST_H
