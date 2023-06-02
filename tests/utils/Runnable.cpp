//
// Created by Stepan Usatiuk on 13.04.2023.
//

#include "Runnable.h"

Runnable::Runnable(std::string name) : name(std::move(name)) {}

std::string Runnable::getName() {
    return name;
}

Runnable::~Runnable() = default;
