//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "ChunkTest.h"
#include "../../src/repo/Serialize.h"
#include "../utils/Cleaner.h"

std::unique_ptr<TestGroup> ChunkTest::operator()() {
    auto tg = std::make_unique<TestGroup>("Chunk tests");

    tg->addTest(std::make_unique<Test>("De/serialize test", std::function<bool()>([]() {
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

                                               HelpfulAssert<Object::idType>()(o1.id, o1e.id);
                                               HelpfulAssert<Object::idType>()(o2.id, o2e.id);
                                               HelpfulAssert<int>()((int) o1.type, (int) o1e.type);
                                               HelpfulAssert<int>()((int) o2.type, (int) o2e.type);

                                               HelpfulAssert<size_t>()(o1.data.size(), o1e.data.size());
                                               HelpfulAssert<size_t>()(o2.data.size(), o2e.data.size());
                                               for (int i = 0; i < o1.data.size(); i++) {
                                                   HelpfulAssert<char>()(o1.data[i], o1e.data[i]);
                                               }
                                               for (int i = 0; i < o2.data.size(); i++) {
                                                   HelpfulAssert<char>()(o2.data[i], o2e.data[i]);
                                               }

                                               for (int i = 0; i < o1.md5.size(); i++) {
                                                   HelpfulAssert<char>()(o1.md5[i], o1e.md5[i]);
                                               }
                                               for (int i = 0; i < o2.md5.size(); i++) {
                                                   HelpfulAssert<char>()(o2.md5[i], o2e.md5[i]);
                                               }
                                           }

                                           return true;
                                       })));

    tg->addTest(std::make_unique<Test>("Garbage throw test", std::function<bool()>([]() {
                                           std::vector<char> e{'a', 'b'};
                                           auto eb = e.cbegin();
                                           try {
                                               Chunk o1(eb, e.cend());
                                           } catch (...) {
                                               return true;
                                           }
                                           throw Exception("Object constructed with garbage data!");
                                       })));
    tg->addTest(std::make_unique<Test>("Garbage throw test2", std::function<bool()>([]() {
                                           std::vector<char> e{'s', 'e', 'm', 'b', 'a', 'a'};
                                           auto eb = e.cbegin();
                                           try {
                                               Chunk o1(eb, e.cend());
                                           } catch (...) {
                                               return true;
                                           }
                                           throw Exception("Object constructed with garbage data!");
                                       })));
    tg->addTest(std::make_unique<Test>("Garbage throw test3", std::function<bool()>([]() {
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
                                                   throw Exception("Object constructed with garbage data!");
                                           }
                                           return true;
                                       })));

    return tg;
}