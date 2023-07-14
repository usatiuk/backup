//
// Created by Stepan Usatiuk on 07.06.2023.
//

#ifndef BACKUP_COMMANDMOUNT_H
#define BACKUP_COMMANDMOUNT_H

#include "Command.h"

class CommandMount : public Command {
public:
    CommandMount();
    void run(Context ctx) override;
    static constexpr std::string_view name{"mount"};
};


#endif//BACKUP_COMMANDMOUNT_H
