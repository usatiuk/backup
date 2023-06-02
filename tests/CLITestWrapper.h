//
// Created by Stepan Usatiuk on 10.05.2023.
//

#ifndef SEMBACKUP_CLITESTWRAPPER_H
#define SEMBACKUP_CLITESTWRAPPER_H

#include "utils/TestGroupGenerator.h"

class CLITestWrapper : public TestGroupGenerator {
public:
    std::unique_ptr<TestGroup> operator()() override;
};

#endif//SEMBACKUP_CLITESTWRAPPER_H
