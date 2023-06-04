//
// Created by Stepan Usatiuk on 15.04.2023.
//

#include "../includes/MD5.h"

#include "../../utils/includes/Exception.h"

std::string MD5::calculate(const std::vector<char> &in) {
    MD5 hasher;
    hasher.feedData(in);
    return hasher.getHash();
}

MD5::MD5() {
    if (!mdctx)
        throw Exception("Can't create hashing context!");

    if (!EVP_DigestInit_ex(mdctx.get(), EVP_md5(), nullptr))
        throw Exception("Can't create hashing context!");
}

void MD5::feedData(const std::vector<char> &in) {
    if (in.empty()) return;
    if (!EVP_DigestUpdate(mdctx.get(), in.data(), in.size()))
        throw Exception("Error hashing!");
}

std::string MD5::getHash() {
    std::array<char, 16> out;
    unsigned int s = 0;

    if (!EVP_DigestFinal_ex(mdctx.get(), reinterpret_cast<unsigned char *>(out.data()), &s))
        throw Exception("Error hashing!");

    if (s != out.size())
        throw Exception("Error hashing!");

    if (!EVP_MD_CTX_reset(mdctx.get()))
        throw Exception("Error hashing!");

    return {out.begin(), out.end()};
}

std::string MD5::calculate(const std::string &in) {
    std::vector<char> tmp(in.begin(), in.end());
    return MD5::calculate(tmp);
}
