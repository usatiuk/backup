//
// Created by Stepan Usatiuk on 15.04.2023.
//

#ifndef SEMBACKUP_MD5TEST_H
#define SEMBACKUP_MD5TEST_H

#include "../../src/crypto/MD5.h"
#include "../utils/TestGroupGenerator.h"

class MD5Test : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_MD5TEST_H
