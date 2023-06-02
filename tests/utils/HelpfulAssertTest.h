//
// Created by Stepan Usatiuk on 13.04.2023.
//

#ifndef SEMBACKUP_HELPFULASSERTTEST_H
#define SEMBACKUP_HELPFULASSERTTEST_H

#include "TestGroupGenerator.h"

class HelpfulAssertTest : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};


#endif//SEMBACKUP_HELPFULASSERTTEST_H
