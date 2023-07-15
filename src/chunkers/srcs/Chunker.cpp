//
// Created by Stepan Usatiuk on 15.04.2023.
//

#include "Chunker.h"

#include "Exception.h"

Chunker::Chunker(std::streambuf *buf, unsigned long long maxBytes) : buf(buf), maxBytes(maxBytes) {}

bool Chunker::getEof() const {
    return eof;
}

Chunker::~Chunker() = default;

Chunker::ChunkerIterator Chunker::begin() {
    return {this};
}

Chunker::ChunkerIterator Chunker::end() {
    return {nullptr};
}

Chunker::ChunkerIterator &Chunker::ChunkerIterator::operator++() {
    if (pastEOF) throw Exception("Trying to increment pastEOF ChunkerIterator!");
    if (source->getEof())
        pastEOF = true;
    else
        buf = source->getNext();
    return *this;
}

bool Chunker::ChunkerIterator::operator!=(const Chunker::ChunkerIterator &rhs) const {
    return pastEOF != rhs.pastEOF;
}

Chunker::ChunkerIterator::value_type Chunker::ChunkerIterator::operator*() const {
    if (pastEOF) throw Exception("Trying to dereference pastEOF ChunkerIterator!");
    return buf.value();
}

bool Chunker::ChunkerIterator::operator==(const Chunker::ChunkerIterator &rhs) const {
    return pastEOF == rhs.pastEOF;
}

Chunker::ChunkerIterator::ChunkerIterator(Chunker *source)
    : source(source), pastEOF(source == nullptr) {
    if (source)
        operator++();
}
