//
// Created by Stepan Usatiuk on 27.04.2023.
//

#ifndef SEMBACKUP_BUZHASHTEST_H
#define SEMBACKUP_BUZHASHTEST_H

#include "utils/TestGroupGenerator.h"

class BuzhashTest : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_BUZHASHTEST_H
