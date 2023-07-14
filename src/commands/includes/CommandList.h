//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMANDLIST_H
#define SEMBACKUP_COMMANDLIST_H

#include "Command.h"

#include "CommandsCommon.h"

/// Lists available archives in a repository
class CommandList : public Command {
public:
    CommandList();
    void run(Context ctx) override;
    static constexpr std::string_view name{"list"};
};


#endif//SEMBACKUP_COMMANDLIST_H
