//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_LOGGER_H
#define SEMBACKUP_LOGGER_H

#include <functional>
#include <iostream>
#include <mutex>

class Logger {
public:
    Logger(int level = 3, std::ostream &out = {std::cout});
    void write(const std::string &what, int whatlevel);
    void setLevel(int level);

private:
    int loglevel;
    std::mutex outM;
    std::reference_wrapper<std::ostream> out;
};


#endif//SEMBACKUP_LOGGER_H
