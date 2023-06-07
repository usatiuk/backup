//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include <gtest/gtest.h>

#include "Cleaner.h"
#include "Serialize.h"
#include "objects/Chunk.h"

TEST(Chunk, Deserialize) {
    std::vector<char> s1, s2;
    {
        std::vector<char> data1{'a', 'b', 'c', 'e'};
        Chunk o1(666, {0}, data1);
        std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
        Chunk o2(777, {1}, data2);

        s1 = Serialize::serialize(o1);
        s2 = Serialize::serialize(o2);
    }
    {
        auto s1b = s1.cbegin();
        auto s2b = s2.cbegin();
        Chunk o1(s1b, s1.cend()), o2(s2b, s2.cend());

        std::vector<char> data1{'a', 'b', 'c', 'e'};
        Chunk o1e(666, {0}, data1);
        std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
        Chunk o2e(777, {1}, data2);

        EXPECT_EQ(o1.id, o1e.id);
        EXPECT_EQ(o2.id, o2e.id);
        EXPECT_EQ((int) o1.type, (int) o1e.type);
        EXPECT_EQ((int) o2.type, (int) o2e.type);

        EXPECT_EQ(o1.data.size(), o1e.data.size());
        EXPECT_EQ(o2.data.size(), o2e.data.size());
        for (int i = 0; i < o1.data.size(); i++) {
            EXPECT_EQ(o1.data[i], o1e.data[i]);
        }
        for (int i = 0; i < o2.data.size(); i++) {
            EXPECT_EQ(o2.data[i], o2e.data[i]);
        }

        for (int i = 0; i < o1.SHA.size(); i++) {
            EXPECT_EQ(o1.SHA[i], o1e.SHA[i]);
        }
        for (int i = 0; i < o2.SHA.size(); i++) {
            EXPECT_EQ(o2.SHA[i], o2e.SHA[i]);
        }
    }
}

TEST(Chunk, Garbage) {
    std::vector<char> e{'a', 'b'};
    auto eb = e.cbegin();
    try {
        Chunk o1(eb, e.cend());
    } catch (...) {
        return;
    }
    FAIL() << "Object constructed with garbage data!";
}

TEST(Chunk, Garbage2) {
    std::vector<char> e{'s', 'e', 'm', 'b', 'a', 'a'};
    auto eb = e.cbegin();
    try {
        Chunk o1(eb, e.cend());
    } catch (...) {
        return;
    }
    FAIL() << "Object constructed with garbage data!";
}

TEST(Chunk, Garbage3) {
    std::vector<char> s1, s2;
    {
        std::vector<char> data1{'a', 'b', 'c', 'e'};
        Chunk o1(666, {0}, data1);
        std::vector<char> data2{'q', 'w', 'e', 'r', 'b'};
        Chunk o2(777, {1}, data2);

        s1 = Serialize::serialize(o1);
        s2 = Serialize::serialize(o2);
    }
    {
        s1.resize(s1.size() - 1);
        s2.resize(s1.size() - 2);
        bool fail = false;

        try {
            Chunk o1 = Serialize::deserialize<Chunk>(s1);
            fail = true;
        } catch (...) {
        }

        try {
            Chunk o2 = Serialize::deserialize<Chunk>(s2);
            fail = true;
        } catch (...) {
        }

        if (fail)
            FAIL() << "Object constructed with garbage data!";
    }
}