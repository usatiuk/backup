//
// Created by Stepan Usatiuk on 15.04.2023.
//

#include "ConstChunker.h"

#include "Exception.h"
#include "SHA.h"

ConstChunker::ConstChunker(std::streambuf *buf, unsigned long long maxBytes) : Chunker(buf, maxBytes) {}

std::pair<std::string, std::vector<char>> ConstChunker::getNext() {
    if (eof) throw Exception("Trying to read from a file that is finished!");

    std::vector<char> rbuf(maxBytes);

    auto read = static_cast<unsigned long>(buf->sgetn(rbuf.data(), (long) maxBytes));

    if (read != maxBytes) {
        eof = true;
        rbuf.resize(read);
    }

    auto SHA = SHA::calculate(rbuf);

    return {SHA, rbuf};
}
