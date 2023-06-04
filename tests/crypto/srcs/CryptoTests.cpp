//
// Created by Stepan Usatiuk on 30.04.2023.
//

#include "AES.h"
#include "MD5.h"

#include <gtest/gtest.h>

TEST(CryptoTests, AES) {
    std::string in = "hello1";
    auto enc = AES::encrypt(std::vector<char>(in.begin(), in.end()), "p1", "e");
    auto dec = AES::decrypt(enc, "p1", "e");
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

TEST(CryptoTests, MD5) {
    std::vector<char> data{'h', 'e', 'l', 'l', 'o'};
    std::array<unsigned char, 16> excepted{0x5d, 0x41, 0x40, 0x2a, 0xbc, 0x4b, 0x2a, 0x76, 0xb9, 0x71, 0x9d, 0x91, 0x10, 0x17, 0xc5, 0x92};

    auto out = MD5::calculate(data);

    EXPECT_EQ(out.size(), 16);
    for (int i = 0; i < out.size(); i++) {
        EXPECT_EQ(static_cast<uint8_t>(out[i]), excepted[i]);
    }
}
