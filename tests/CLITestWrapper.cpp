//
// Created by Stepan Usatiuk on 10.05.2023.
//

#include "CLITestWrapper.h"
#include "utils/Cleaner.h"

std::unique_ptr<TestGroup> CLITestWrapper::operator()() {
    auto tg = std::make_unique<TestGroup>("CLI tests");

    tg->addTest(std::make_unique<Test>("Test backup", std::function<bool()>([]() {
                                           int ret = system("../tests/clitests/backup.sh");
                                           HelpfulAssert<int>()(WEXITSTATUS(ret), 0);
                                           return true;
                                       })));
    tg->addTest(std::make_unique<Test>("Test ignore", std::function<bool()>([]() {
                                           int ret = system("../tests/clitests/ignore.sh");
                                           HelpfulAssert<int>()(WEXITSTATUS(ret), 0);
                                           return true;
                                       })));

    tg->addTest(std::make_unique<Test>("Test diff", std::function<bool()>([]() {
                                           int ret = system("../tests/clitests/diff.sh");
                                           HelpfulAssert<int>()(WEXITSTATUS(ret), 0);
                                           return true;
                                       })));


    return tg;
}
