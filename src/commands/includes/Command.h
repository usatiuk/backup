//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMAND_H
#define SEMBACKUP_COMMAND_H

#include "Context.h"

/// Abstract base class for some process running with some Context
class Command {
public:
    /// Runs the command with Context \p ctx
    virtual void run(Context ctx) = 0;

    /// Default virtual destructor
    virtual ~Command() = 0;

    /// The name of the command
    const std::string name;

protected:
    /// Constructs a command with name \p name
    Command(std::string name);
};


#endif//SEMBACKUP_COMMAND_H
