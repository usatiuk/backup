//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_CONTEXT_H
#define SEMBACKUP_CONTEXT_H

#include "Config.h"
#include "Logger.h"
#include "repo/Repository.h"

struct Context {
    Logger *logger;
    Repository *repo;
};


#endif//SEMBACKUP_CONTEXT_H
