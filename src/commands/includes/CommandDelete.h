//
// Created by Stepan Usatiuk on 06.08.2023.
//

#ifndef BACKUP_COMMANDDELETE_H
#define BACKUP_COMMANDDELETE_H

#include "Command.h"

class CommandDelete : public Command {
public:
    CommandDelete();
    void run(Context ctx) override;
    static constexpr std::string_view name{"delete"};
};


#endif//BACKUP_COMMANDDELETE_H
