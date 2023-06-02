//
// Created by Stepan Usatiuk on 12.05.2023.
//

#include "CheckFilter.h"
#include "../crypto/CRC32.h"
#include "../repo/Serialize.h"

std::vector<char> CheckFilter::filterWrite(std::vector<char> from) const {
    return filterWriteStatic(std::move(from));
}

std::vector<char> CheckFilter::filterRead(std::vector<char> from) const {
    return filterReadStatic(std::move(from));
}

std::vector<char> CheckFilter::filterWriteStatic(std::vector<char> from) {
    auto out = magic;

    Serialize::serialize(from, out);

    auto crc = CRC32::calculate(from);

    Serialize::serialize(crc, out);

    return out;
}

std::vector<char> CheckFilter::filterReadStatic(std::vector<char> from) {
    if (from.size() < magic.size()) throw Exception("Input is corrupted (too small)!");

    for (size_t i = 0; i < magic.size(); i++) {
        if (from[i] != magic[i]) throw Exception("Magic prefix is wrong!");
    }

    auto fromIt = from.cbegin() + magic.size();

    auto out = Serialize::deserialize<std::vector<char>>(fromIt, from.cend());

    auto crc = CRC32::calculate(out);

    auto crcRecorded = Serialize::deserialize<CRC32::crcType>(fromIt, from.cend());

    if (crc != crcRecorded) throw Exception("CRC mismatch!");

    return out;
}
