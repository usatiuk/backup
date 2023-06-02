//
// Created by Stepan Usatiuk on 15.04.2023.
//

#include "MD5Test.h"

std::unique_ptr<TestGroup> MD5Test::operator()() {
    auto tg = std::make_unique<TestGroup>("MD5 tests");

    tg->addTest(std::make_unique<Test>("Simple MD5 test", std::function<bool()>([]() {
                                           std::vector<char> data{'h', 'e', 'l', 'l', 'o'};
                                           std::array<unsigned char, 16> excepted{0x5d, 0x41, 0x40, 0x2a, 0xbc, 0x4b, 0x2a, 0x76, 0xb9, 0x71, 0x9d, 0x91, 0x10, 0x17, 0xc5, 0x92};

                                           auto out = MD5::calculate(data);

                                           HelpfulAssert<size_t>()(out.size(), 16);
                                           for (int i = 0; i < out.size(); i++) {
                                               HelpfulAssert<unsigned char>()(out[i], excepted[i]);
                                           }


                                           return true;
                                       })));
    return tg;
}
