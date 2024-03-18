//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "Repository.h"
Repository::~Repository() = default;

Repository::Repository(Config config) : config(std::move(config)) {}

const Config &Repository::getConfig() const { return config; }

std::unique_ptr<Object> Repository::getObject(Object::idType id) const {
    return Object::deserialize(this->getObjectRaw(id));
}
