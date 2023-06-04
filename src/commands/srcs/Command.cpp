//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "../includes/Command.h"

Command::Command(std::string name) : name(std::move(name)) {}

Command::~Command() = default;