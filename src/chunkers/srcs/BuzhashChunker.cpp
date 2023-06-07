//
// Created by Stepan Usatiuk on 26.04.2023.
//

#include "../includes/BuzhashChunker.h"

#include "../../crypto/includes/SHA.h"
#include "../../utils/includes/Exception.h"

BuzhashChunker::BuzhashChunker(std::streambuf *buf, unsigned long long minBytes, unsigned long long maxBytes, unsigned long long mask, uint32_t window) : Chunker(buf, maxBytes), window(window), minBytes(minBytes), mask(mask), buzhash(window) {}

std::pair<std::string, std::vector<char>> BuzhashChunker::getNext() {
    if (eof) throw Exception("Trying to read from a file that is finished!");
    std::vector<char> rbuf(minBytes);

    auto read = static_cast<unsigned long>(buf->sgetn(rbuf.data(), (long) minBytes));

    if (read != minBytes) {
        eof = true;
        rbuf.resize(read);
        return {SHA::calculate(rbuf), rbuf};
    }

    for (auto c: rbuf) {
        buzhash.feed(static_cast<uint8_t>(c));
    }

    // Continue reading the file until either the last mask bits are zero of we exceed the maxSize
    while (((buzhash.get() & (~0UL >> (sizeof(unsigned long long) * 8 - mask))) != 0) && rbuf.size() < maxBytes) {
        auto r = buf->sbumpc();
        if (r == std::streambuf::traits_type::eof()) {
            eof = true;
            break;
        } else {
            char c = std::streambuf::traits_type::to_char_type(r);
            rbuf.emplace_back(c);
            buzhash.feed(static_cast<uint8_t>(c));
        }
    }

    return {SHA::calculate(rbuf), rbuf};
}
