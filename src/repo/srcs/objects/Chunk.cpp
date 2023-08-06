//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "objects/Chunk.h"

#include "Exception.h"
#include "Serialize.h"

Chunk::Chunk(idType id, std::string SHA, std::vector<char> data)
    : Object(id, ObjectType::Chunk), data(std::move(data)), SHA(std::move(SHA)), length(this->data.size()) {}

Chunk::Chunk(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end)
    : Object(in, end), SHA(Serialize::deserialize<std::remove_const<decltype(SHA)>::type>(in, end)),
      data(Serialize::deserialize<std::remove_const<decltype(data)>::type>(in, end)),
      length(Serialize::deserialize<std::remove_const<decltype(length)>::type>(in, end)) {
    if (type != ObjectType::Chunk) throw Exception("Type mismatch for Chunk!");
    if (length != data.size()) throw Exception("Recorded length and actual length don't match for Chunk!");
}

void Chunk::serialize(std::vector<char> &out) const {
    Object::serialize(out);
    Serialize::serialize(SHA, out);
    Serialize::serialize(data, out);
    Serialize::serialize(length, out);
}

std::string Chunk::getKey() const { return SHA; }
