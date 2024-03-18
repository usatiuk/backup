//
// Created by Stepan Usatiuk on 06.08.2023.
//

#include "CommandDelete.h"

#include "CommandsCommon.h"

using namespace CommandsCommon;

CommandDelete::CommandDelete() {}

void CommandDelete::run(Context ctx) {
    ctx.repo->deleteObjects({static_cast<unsigned long long>(ctx.repo->getConfig().getInt("aid"))});
}
