//
// Created by Stepan Usatiuk on 23.04.2023.
//

#include "FilterAES.h"

#include "../crypto/AES.h"

std::vector<char> FilterAES::filterWrite(std::vector<char> from) const {
    return AES::encrypt(from, key);
}

std::vector<char> FilterAES::filterRead(std::vector<char> from) const {
    return AES::decrypt(from, key);
}

FilterAES::FilterAES(const std::string &password, const std::string &salt) : key(AES::deriveKey(password, salt)) {}
