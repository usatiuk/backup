//
// Created by Stepan Usatiuk on 30.04.2023.
//

#include "AES.h"
#include "SHA.h"

#include <gtest/gtest.h>

TEST(CryptoTests, AES) {
    std::string in = "hello1";
    auto enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    auto dec = AES::decrypt(enc, "p1", "e");
    EXPECT_EQ(in, std::string(dec.begin(), dec.end()));
    
    in = "";
    enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    dec = AES::decrypt(enc, "p1", "e");
    EXPECT_EQ(in, std::string(dec.begin(), dec.end()));

    in = "1234567890asdfg";
    enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    dec = AES::decrypt(enc, "p1", "e");
    EXPECT_EQ(in, std::string(dec.begin(), dec.end()));

    in = "1234567890asdfgh";
    enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    dec = AES::decrypt(enc, "p1", "e");
    EXPECT_EQ(in, std::string(dec.begin(), dec.end()));

    in = "1234567890asdfghe";
    enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    dec = AES::decrypt(enc, "p1", "e");
    EXPECT_EQ(in, std::string(dec.begin(), dec.end()));

    in = "1234567890asdfgheq";
    enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    dec = AES::decrypt(enc, "p1", "e");
    EXPECT_EQ(in, std::string(dec.begin(), dec.end()));
}

TEST(CryptoTests, SHA) {
    std::vector<char> data{'h', 'e', 'l', 'l', 'o'};
    std::array<unsigned char, 32> excepted{0x2c, 0xf2, 0x4d, 0xba, 0x5f, 0xb0, 0xa3, 0x0e, 0x26, 0xe8, 0x3b, 0x2a, 0xc5, 0xb9, 0xe2, 0x9e, 0x1b, 0x16, 0x1e, 0x5c, 0x1f, 0xa7, 0x42, 0x5e, 0x73, 0x04, 0x33, 0x62, 0x93, 0x8b, 0x98, 0x24};

    auto out = SHA::calculate(data);

    EXPECT_EQ(out.size(), 32);
    for (int i = 0; i < out.size(); i++) {
        EXPECT_EQ(static_cast<uint8_t>(out[i]), excepted[i]);
    }
}
