//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "Object.h"

#include "Serialize.h"

#include "objects/Archive.h"
#include "objects/Chunk.h"
#include "objects/File.h"

Object::Object(idType id, ObjectType type) : id(id), type(type) {}

Object::Object(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end)
    : id(Serialize::deserialize<idType>(in, end)), type(Serialize::deserialize<ObjectType>(in, end)) {}

void Object::serialize(std::vector<char> &out) const {
    Serialize::serialize(id, out);
    Serialize::serialize(type, out);
}

Object::~Object() = default;

static std::vector<Object::idType> emptyRef{};

const std::vector<Object::idType> &Object::getRefs() const { return emptyRef; }

std::unique_ptr<Object> Object::deserialize(std::vector<char>::const_iterator &in,
                                            const std::vector<char>::const_iterator &end) {
    auto inCpy = in;
    auto id = Serialize::deserialize<idType>(in, end);
    auto type = Serialize::deserialize<ObjectType>(in, end);

    switch (type) {
        case ObjectType::Archive:
            return std::make_unique<Archive>(Serialize::deserialize<Archive>(inCpy, end));
        case ObjectType::File:
            return std::make_unique<File>(Serialize::deserialize<File>(inCpy, end));
        case ObjectType::Chunk:
            return std::make_unique<Chunk>(Serialize::deserialize<Chunk>(inCpy, end));
        case ObjectType::END:
            break;
        default:
            throw Exception("Bad object!");
    }
}

std::unique_ptr<Object> Object::deserialize(const std::vector<char> &src) {
    auto srcIterator = src.cbegin();
    return deserialize(srcIterator, src.end());
}
