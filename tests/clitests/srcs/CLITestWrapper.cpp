//
// Created by Stepan Usatiuk on 10.05.2023.
//

#include "Cleaner.h"
#include <gtest/gtest.h>

//TODO: Don't hardcode scripts

TEST(CLITest, Backup) {
    int ret = system("../../../tests/clitests/scripts/backup.sh");
    EXPECT_EQ(WEXITSTATUS(ret), 0);
}

TEST(CLITest, Ignore) {
    int ret = system("../../../tests/clitests/scripts/ignore.sh");
    EXPECT_EQ(WEXITSTATUS(ret), 0);
}

TEST(CLITest, Diff) {
    int ret = system("../../../tests/clitests/scripts/diff.sh");
    EXPECT_EQ(WEXITSTATUS(ret), 0);
}