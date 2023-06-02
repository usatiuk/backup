//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "Chunk.h"

#include "../../Exception.h"
#include "../Serialize.h"

Chunk::Chunk(idType id, std::string md5, std::vector<char> data) : Object(id, ObjectType::Chunk), data(std::move(data)), md5(std::move(md5)), length(this->data.size()) {}

Chunk::Chunk(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end)
    : Object(in, end),
      md5(Serialize::deserialize<std::remove_const<decltype(md5)>::type>(in, end)),
      data(Serialize::deserialize<std::remove_const<decltype(data)>::type>(in, end)),
      length(Serialize::deserialize<std::remove_const<decltype(length)>::type>(in, end)) {
    if (type != ObjectType::Chunk) throw Exception("Type mismatch for Chunk!");
    if (length != data.size()) throw Exception("Recorded length and actual length don't match for Chunk!");
}

void Chunk::serialize(std::vector<char> &out) const {
    Object::serialize(out);
    Serialize::serialize(md5, out);
    Serialize::serialize(data, out);
    Serialize::serialize(length, out);
}

std::string Chunk::getKey() const {
    return md5;
}
