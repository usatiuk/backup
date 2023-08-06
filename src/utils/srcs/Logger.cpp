//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "Logger.h"

Logger::Logger(int level, std::ostream &out) : loglevel(level), out(out) {}

void Logger::write(const std::string &what, int whatlevel) {
    if (whatlevel <= loglevel) {
        std::lock_guard outLock(outM);
        out.get() << what << std::flush;
    }
}

void Logger::setLevel(int level) { loglevel = level; }
