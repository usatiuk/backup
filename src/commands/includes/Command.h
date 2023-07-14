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
    virtual ~Command() = default;
};


#endif//SEMBACKUP_COMMAND_H
