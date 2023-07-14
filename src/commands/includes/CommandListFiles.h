//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMANDLISTFILES_H
#define SEMBACKUP_COMMANDLISTFILES_H

#include "Command.h"

#include "CommandsCommon.h"

/// Lists files in the selected Archive
class CommandListFiles : public Command {
public:
    CommandListFiles();
    void run(Context ctx) override;
    static constexpr std::string_view name{"list-files"};
};


#endif//SEMBACKUP_COMMANDLISTFILES_H
