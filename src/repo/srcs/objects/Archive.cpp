//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "objects/Archive.h"

#include "Exception.h"
#include "Serialize.h"

Archive::Archive(Object::idType id, std::string name, unsigned long long mtime, std::vector<idType> files)
    : Object(id, ObjectType::Archive), name(name), mtime(mtime), files(files) {}

Archive::Archive(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end)
    : Object(in, end), name(Serialize::deserialize<std::string>(in, end)),
      mtime(Serialize::deserialize<unsigned long long>(in, end)),
      files(Serialize::deserialize<std::remove_const<decltype(files)>::type>(in, end)) {
    if (type != ObjectType::Archive) throw Exception("Type mismatch for Archive!");
    auto filesN = Serialize::deserialize<decltype(files.size())>(in, end);
    if (files.size() != filesN) throw Exception("Number of files recorded doesn't match the number of files read!");
}

void Archive::serialize(std::vector<char> &out) const {
    Object::serialize(out);
    Serialize::serialize(name, out);
    Serialize::serialize(mtime, out);
    Serialize::serialize(files, out);
    Serialize::serialize(files.size(), out);
}

std::string Archive::getKey() const { return name; }

const std::vector<Object::idType> &Archive::getRefs() const { return files; }
