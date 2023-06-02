//
// Created by Stepan Usatiuk on 13.04.2023.
//

#ifndef SEMBACKUP_RUNNABLE_H
#define SEMBACKUP_RUNNABLE_H

#include <iostream>
#include <string>

class Runnable {
public:
    Runnable(std::string name);

    virtual bool run(std::ostream &out) = 0;

    virtual ~Runnable();

    std::string getName();

protected:
    std::string name;
};


#endif//SEMBACKUP_RUNNABLE_H
