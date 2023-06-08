//
// Created by Stepan Usatiuk on 07.06.2023.
//

#include "../includes/CommandMount.h"
#include "RepoFS.h"

CommandMount::CommandMount() : Command("mount") {
}

void CommandMount::run(Context ctx) {
    RepoFS::start(ctx.repo, "./hi");
}
