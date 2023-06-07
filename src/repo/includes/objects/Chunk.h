//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_CHUNK_H
#define SEMBACKUP_CHUNK_H

#include <array>
#include <vector>

#include "../Object.h"

/// Object representing a part of a File
class Chunk : public Object {
public:
    Chunk(idType id, std::string, std::vector<char> data);

    /// Deserialization constructor
    Chunk(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end);

    /// \copydoc Object::serialize
    void serialize(std::vector<char> &out) const override;

    /// Returns the SHA of the chunk
    std::string getKey() const override;

    const std::string SHA;          ///< SHA hash of the chunk
    const std::vector<char> data;   ///< Raw chunk data
    const unsigned long long length;///< Size of chunk in bytes
};


#endif//SEMBACKUP_CHUNK_H
