//
// Created by Stepan Usatiuk on 27.04.2023.
//

#include "Buzhash.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(BuzhashTest, SimpleTest) {
    std::string loremipsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    for (int i = 15; i < 49; i++) {
        Buzhash b(i);
        std::string p1 = "reprehenderit in voluptate velit esse cillum dolore";
        for (auto c: p1) b.feed(static_cast<uint8_t>(c));
        auto h1 = b.get();

        Buzhash b2(i);
        bool h1found = false;

        for (int i = 0; i < loremipsum.length(); i++) {
            b2.feed((uint8_t) loremipsum[i]);
            if (b2.get() == h1) {
                EXPECT_EQ(i, loremipsum.find("e eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non p"));
                h1found = true;
                break;
            }
        }
        EXPECT_EQ(h1found, true);
    }
}
