//
// Created by Stepan Usatiuk on 26.04.2023.
//

#ifndef SEMBACKUP_BUZHASHCHUNKER_H
#define SEMBACKUP_BUZHASHCHUNKER_H

#include <streambuf>

#include "Buzhash.h"
#include "Chunker.h"

/// Chunker implementation using rolling hash
class BuzhashChunker : public Chunker {
public:
    /// Constructs a BuzhashChunker
    /// \copydoc Chunker::Chunker
    /// \param minBytes Minimum amount of bytes in returned chunks
    /// \param mask     Amount of trailing zeroes in the rolling hash at which the file is cut (results in average chunk size of 2^mask bytes)
    /// \param window   Rolling hash window (how many of chunks last bytes are included in the hash, the default is recommended)
    BuzhashChunker(std::streambuf *buf, unsigned long long minBytes, unsigned long long maxBytes,
                   unsigned long long mask, uint32_t window = 4095);

    /// \copydoc Chunker::getNext
    std::pair<std::string, std::vector<char>> getNext() override;

private:
    const unsigned long long window;  ///< Rolling hash window
    const unsigned long long minBytes;///< Minimum amount of bytes in returned chunks
    const unsigned long long mask;    ///< Amount of trailing zeroes in the rolling hash at which the file is cut
    Buzhash buzhash;                  ///< Hasher instance
};


#endif//SEMBACKUP_BUZHASHCHUNKER_H
