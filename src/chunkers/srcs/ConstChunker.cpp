//
// Created by Stepan Usatiuk on 15.04.2023.
//

#include "../includes/ConstChunker.h"

#include "../../crypto/includes/MD5.h"
#include "../../utils/includes/Exception.h"

ConstChunker::ConstChunker(std::streambuf *buf, unsigned long long maxBytes) : Chunker(buf, maxBytes) {}

std::pair<std::string, std::vector<char>> ConstChunker::getNext() {
    if (eof) throw Exception("Trying to read from a file that is finished!");

    std::vector<char> rbuf(maxBytes);

    auto read = static_cast<unsigned long>(buf->sgetn(rbuf.data(), (long) maxBytes));

    if (read != maxBytes) {
        eof = true;
        rbuf.resize(read);
    }

    auto md5 = MD5::calculate(rbuf);

    return {md5, rbuf};
}
