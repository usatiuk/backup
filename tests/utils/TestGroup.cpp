//
// Created by Stepan Usatiuk on 13.04.2023.
//

#include "TestGroup.h"

TestGroup::TestGroup(std::string name) : Runnable(std::move(name)) {}

bool TestGroup::run(std::ostream &out) {
    bool ok = true;
    out << "Running test group " << getName() << std::endl;
    for (auto const &p: tests) {
        out << "  Running " << p->getName() << " ";
        try {
            p->run(out);
            out << "OK";
        } catch (const std::exception &e) {
            ok = false;
            out << "ERROR" << std::endl;
            out << e.what();
        }
        out << std::endl;
    }
    return ok;
}

void TestGroup::addTest(std::unique_ptr<Runnable> &&test) {
    tests.emplace_back(std::move(test));
}

TestGroup::~TestGroup() = default;
