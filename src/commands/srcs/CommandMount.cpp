//
// Created by Stepan Usatiuk on 07.06.2023.
//

#include "CommandMount.h"
#include "RepoFS.h"

CommandMount::CommandMount() : Command() {
}

void CommandMount::run(Context ctx) {
    RepoFS::start(ctx.repo, ctx.repo->getConfig().getStr("to"));
}
