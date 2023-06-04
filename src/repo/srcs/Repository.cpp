//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "../includes/Repository.h"
Repository::~Repository() = default;

Repository::Repository(Config config) : config(std::move(config)) {}

const Config &Repository::getConfig() const {
    return config;
}
