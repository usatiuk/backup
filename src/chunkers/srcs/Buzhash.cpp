//
// Created by Stepan Usatiuk on 26.04.2023.
//

#include "Buzhash.h"

Buzhash::Buzhash(uint32_t blockSize) : blockSize(blockSize), history() {}

uint32_t Buzhash::get() const { return cur; }

uint32_t Buzhash::feed(uint8_t in) {
    cur = rotr32(cur, 1);

    if (history.size() >= blockSize) {
        auto oldest = history.back();
        history.pop_back();
        cur ^= rotr32(randomNumbers[oldest], blockSize);
    }

    history.emplace_front(in);

    cur ^= randomNumbers[in];

    return cur;
}

// Circular shift taken from: https://en.wikipedia.org/wiki/Circular_shift
uint32_t Buzhash::rotr32(uint32_t value, unsigned int count) {
    const unsigned int mask = CHAR_BIT * sizeof(value) - 1;
    count &= mask;
    return (value >> count) | (value << (-count & mask));
}
