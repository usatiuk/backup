//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMANDDIFF_H
#define SEMBACKUP_COMMANDDIFF_H

#include "Command.h"

#include "CommandsCommon.h"

/// Run the diff between:
/// 1. The latest archive and the `from` directory
/// 2. if `aid` is set the aid archive and the `from` directory
/// 3. if `aid` and `aid2` are set between `aid` and `aid2`
class CommandDiff : public Command {
public:
    CommandDiff();
    void run(Context ctx) override;
    static constexpr std::string_view name{"diff"};
};


#endif//SEMBACKUP_COMMANDDIFF_H
