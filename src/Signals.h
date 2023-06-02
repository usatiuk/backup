//
// Created by Stepan Usatiuk on 16.04.2023.
//

#ifndef SEMBACKUP_SIGNALS_H
#define SEMBACKUP_SIGNALS_H

#include <csignal>

/// Class to handle signals sent to the process
class Signals {
public:
    /// Setup the signal handlers
    static void setup();

    volatile static inline std::sig_atomic_t shouldQuit = false;///< Indicates whether the program was requested to exit

private:
    /// Handle the signals
    static void handle(int signum);
};


#endif//SEMBACKUP_SIGNALS_H
