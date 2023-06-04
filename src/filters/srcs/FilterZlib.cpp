//
// Created by Stepan Usatiuk on 23.04.2023.
//

#include "../includes/FilterZlib.h"

#include <zlib.h>

#include "../../repo/includes/Serialize.h"

std::vector<char> FilterZlib::filterWrite(std::vector<char> from) const {
    uLongf outSize = compressBound(from.size());

    std::vector<char> out;
    Serialize::serialize('C', out);
    Serialize::serialize(static_cast<unsigned long long>(from.size()), out);

    uLongf sizeSize = out.size();

    out.resize(sizeSize + outSize);

    if (compress2(reinterpret_cast<Bytef *>(out.data() + sizeSize), &outSize, reinterpret_cast<const Bytef *>(from.data()), from.size(), level) !=
        Z_OK)
        throw Exception("Error compressing!");

    out.resize(outSize + sizeSize);

    return out;
}

std::vector<char> FilterZlib::filterRead(std::vector<char> from) const {
    auto desI = from.cbegin();

    char C = Serialize::deserialize<char>(desI, from.cend());
    if (C != 'C') throw Exception("Bad compression prefix!");

    uLongf size = Serialize::deserialize<unsigned long long>(desI, from.cend());

    std::vector<char> out(size);

    if (desI >= from.cend()) throw Exception("Unexpected end of archive!");

    if (uncompress(reinterpret_cast<Bytef *>(out.data()), &size, reinterpret_cast<const Bytef *>(&(*desI)), std::distance(desI, from.cend())) !=
        Z_OK)
        throw Exception("Error decompressing!");

    return out;
}

FilterZlib::FilterZlib(int level) : level(level) {}
