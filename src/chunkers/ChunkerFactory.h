//
// Created by Stepan Usatiuk on 30.04.2023.
//

#ifndef SEMBACKUP_CHUNKERFACTORY_H
#define SEMBACKUP_CHUNKERFACTORY_H

#include <memory>
#include <streambuf>

#include "../Config.h"
#include "Chunker.h"

/// Factory for Chunker%s
class ChunkerFactory {
public:
    /// Creates a new Chunker based on provided \p config backed with \p buf
    /// \param config Constant reference to Config
    /// \param buf    Pointer to a std::streambuf instance, should be avaliable during the Chunker lifetime
    /// \return       Unique pointer to the created Chunker
    static std::unique_ptr<Chunker> getChunker(const Config &config, std::streambuf *buf);
};


#endif//SEMBACKUP_CHUNKERFACTORY_H
