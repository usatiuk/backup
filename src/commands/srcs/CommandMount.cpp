//
// Created by Stepan Usatiuk on 07.06.2023.
//

#include "../includes/CommandMount.h"
#include "RepoFS.h"

CommandMount::CommandMount() : Command("mount") {
}

void CommandMount::run(Context ctx) {
    RepoFS rfs(ctx.repo, ctx.repo->getObjects(Object::ObjectType::Archive).begin()->second, "./hi");
    rfs.workerFn();
}
