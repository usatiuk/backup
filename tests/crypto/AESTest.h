//
// Created by Stepan Usatiuk on 30.04.2023.
//

#ifndef SEMBACKUP_AESTEST_H
#define SEMBACKUP_AESTEST_H

#include "../utils/TestGroupGenerator.h"

class AESTest : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_AESTEST_H
