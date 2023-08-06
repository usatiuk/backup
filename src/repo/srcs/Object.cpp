//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "Object.h"

#include "Serialize.h"

Object::Object(idType id, ObjectType type) : id(id), type(type) {}

Object::Object(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end)
    : id(Serialize::deserialize<idType>(in, end)), type(Serialize::deserialize<ObjectType>(in, end)) {}

void Object::serialize(std::vector<char> &out) const {
    Serialize::serialize(id, out);
    Serialize::serialize(type, out);
}

Object::~Object() = default;
