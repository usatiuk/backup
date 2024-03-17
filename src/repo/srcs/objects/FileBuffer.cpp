//
// Created by Stepan Usatiuk on 23.04.2023.
//

#include "objects/FileBuffer.h"

#include "Serialize.h"

FileBuffer::FileBuffer(const Repository *repo, Object::idType fileId)
    : repo(repo), file(Serialize::deserialize<File>(repo->getObjectRaw(fileId))), chunksQueue() {
    for (auto const &id: file.chunks) chunksQueue.emplace(id.second);
};

int FileBuffer::sync() { return 0; }

std::streamsize FileBuffer::xsgetn(char *s, std::streamsize countr) {
    if (underflow() == std::char_traits<char>::eof()) return 0;
    for (int i = 0; i < countr; i++) {
        auto c = uflow();
        if (c != traits_type::eof()) {
            s[i] = traits_type::to_char_type(c);
        } else
            return i;
    }
    return countr;
}

int FileBuffer::uflow() {
    auto out = underflow();
    if (out != traits_type::eof()) curGetBufPos++;
    return out;
}

int FileBuffer::underflow() {
    if (getBuf.empty() || curGetBufPos == getBuf.size()) {
        if (chunksQueue.empty()) return traits_type::eof();
        else {
            auto chunk = Serialize::deserialize<Chunk>(repo->getObjectRaw(chunksQueue.front()));
            getBuf = chunk.data;
            chunksQueue.pop();
            curGetBufPos = 0;
        }
    }

    if (!getBuf.empty()) return traits_type::to_int_type(getBuf[curGetBufPos]);
    else
        return traits_type::eof();
}
