//
// Created by Stepan Usatiuk on 13.04.2023.
//

#include <memory>
#include <vector>

#include "BuzhashTest.h"
#include "CLITestWrapper.h"
#include "crypto/AESTest.h"
#include "crypto/MD5Test.h"
#include "fulltests/FullTest.h"
#include "repo/ChunkTest.h"
#include "repo/FileRepositoryTest.h"
#include "utils/HelpfulAssertTest.h"

int main() {
    std::vector<std::unique_ptr<Runnable>> tests{};
    tests.emplace_back(HelpfulAssertTest()());
    tests.emplace_back(MD5Test()());
    tests.emplace_back(AESTest()());
    tests.emplace_back(ChunkTest()());
    tests.emplace_back(FileRepositoryTest()());
    tests.emplace_back(BuzhashTest()());
    tests.emplace_back(CLITestWrapper()());
    tests.emplace_back(FullTest()());

    bool ok = true;
    for (const auto &t: tests) {
        ok = t->run(std::cout) && ok;
    }

    return ok ? 0 : -1;
}