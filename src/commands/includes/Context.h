//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_CONTEXT_H
#define SEMBACKUP_CONTEXT_H

#include "../../repo/includes/Repository.h"
#include "../../utils/includes/Config.h"
#include "../../utils/includes/Logger.h"

struct Context {
    Logger *logger;
    Repository *repo;
};


#endif//SEMBACKUP_CONTEXT_H
