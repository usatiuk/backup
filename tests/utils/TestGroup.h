//
// Created by Stepan Usatiuk on 13.04.2023.
//

#ifndef SEMBACKUP_TESTGROUP_H
#define SEMBACKUP_TESTGROUP_H

#include <memory>
#include <vector>

#include "Runnable.h"

class TestGroup : public Runnable {
public:
    TestGroup(std::string name);

    bool run(std::ostream &out) override;

    void addTest(std::unique_ptr<Runnable> &&test);

    ~TestGroup() override;

private:
    std::vector<std::unique_ptr<Runnable>> tests;
};


#endif//SEMBACKUP_TESTGROUP_H
