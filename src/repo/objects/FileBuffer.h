//
// Created by Stepan Usatiuk on 23.04.2023.
//

#ifndef SEMBACKUP_FILEBUFFER_H
#define SEMBACKUP_FILEBUFFER_H

#include <queue>
#include <streambuf>

#include "../Repository.h"
#include "Chunk.h"
#include "File.h"

/// Streambuf implementation to read files from a File in a Repository
class FileBuffer : public std::streambuf {
public:
    /// Creates a FileBuffer instance
    /// \param repo    Constant pointer to the backing Repository, should be available during the entire lifetime
    /// \param fileId  ID of a file to "open"
    FileBuffer(const Repository *repo, Object::idType fileId);

protected:
    int underflow() override;

    int uflow() override;

    std::streamsize xsgetn(char *s, std::streamsize count) override;

    int sync() override;

private:
    std::vector<char> getBuf;///< Currently loaded chunk

    size_t curGetBufPos = 0;///< Currently pointed to byte in the loaded chunk

    const Repository *repo;                ///< Pointer to the backing repository
    File file;                             ///< Backing file
    std::queue<Object::idType> chunksQueue;///< Chunks of file that weren't read yet
};


#endif//SEMBACKUP_FILEBUFFER_H
