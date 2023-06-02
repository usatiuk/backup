//
// Created by Stepan Usatiuk on 13.04.2023.
//

#ifndef SEMBACKUP_TESTGROUPGENERATOR_H
#define SEMBACKUP_TESTGROUPGENERATOR_H

#include <cassert>
#include <functional>
#include <memory>
#include <sstream>

#include "HelpfulAssert.h"
#include "Runnable.h"
#include "Test.h"
#include "TestGroup.h"

class TestGroupGenerator {
public:
    virtual std::unique_ptr<TestGroup> operator()() = 0;
};

#endif//SEMBACKUP_TESTGROUPGENERATOR_H
