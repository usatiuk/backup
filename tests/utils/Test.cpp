//
// Created by Stepan Usatiuk on 13.04.2023.
//

#include "Test.h"

#include <utility>

Test::Test(std::string name, std::function<bool()> test)
    : Runnable(std::move(name)), test(std::move(test)) {}

bool Test::run(std::ostream &out) {
    return test();
}

Test::~Test() = default;