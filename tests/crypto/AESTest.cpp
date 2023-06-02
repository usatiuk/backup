//
// Created by Stepan Usatiuk on 30.04.2023.
//

#include "AESTest.h"
#include "../../src/crypto/AES.h"

std::unique_ptr<TestGroup> AESTest::operator()() {
    auto tg = std::make_unique<TestGroup>("AES tests");

    tg->addTest(std::make_unique<Test>("Simple AESTest test", std::function<bool()>([]() {
                                           std::string in = "hello1";
                                           auto enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
                                           auto dec = AES::decrypt(enc, "p1", "e");
                                           HelpfulAssert<std::string>()(in, std::string(dec.begin(), dec.end()));

                                           in = "1234567890asdfg";
                                           enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
                                           dec = AES::decrypt(enc, "p1", "e");
                                           HelpfulAssert<std::string>()(in, std::string(dec.begin(), dec.end()));

                                           in = "1234567890asdfgh";
                                           enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
                                           dec = AES::decrypt(enc, "p1", "e");
                                           HelpfulAssert<std::string>()(in, std::string(dec.begin(), dec.end()));

                                           in = "1234567890asdfghe";
                                           enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
                                           dec = AES::decrypt(enc, "p1", "e");
                                           HelpfulAssert<std::string>()(in, std::string(dec.begin(), dec.end()));

                                           in = "1234567890asdfgheq";
                                           enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
                                           dec = AES::decrypt(enc, "p1", "e");
                                           HelpfulAssert<std::string>()(in, std::string(dec.begin(), dec.end()));

                                           return true;
                                       })));
    return tg;
}
