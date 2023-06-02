//
// Created by Stepan Usatiuk on 13.04.2023.
//

#ifndef SEMBACKUP_TEST_H
#define SEMBACKUP_TEST_H

#include <functional>

#include "Runnable.h"

class Test : public Runnable {
public:
    Test(std::string name, std::function<bool()> test);

    bool run(std::ostream &out) override;

    ~Test() override;

private:
    std::function<bool()> test;
};


#endif//SEMBACKUP_TEST_H
