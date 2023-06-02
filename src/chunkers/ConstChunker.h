//
// Created by Stepan Usatiuk on 15.04.2023.
//

#ifndef SEMBACKUP_CONSTCHUNKER_H
#define SEMBACKUP_CONSTCHUNKER_H

#include <streambuf>

#include "Chunker.h"

/// Chunker implementation that splits the file into equally-sized chunks of maxBytes bytes
class ConstChunker : public Chunker {
public:
    /// Constructs a ConstChunker
    /// \copydoc Chunker::Chunker
    ConstChunker(std::streambuf *buf, unsigned long long maxBytes);

    /// \copydoc Chunker::getNext
    std::pair<std::string, std::vector<char>> getNext() override;
};


#endif//SEMBACKUP_CONSTCHUNKER_H
