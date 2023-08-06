//
// Created by Stepan Usatiuk on 16.04.2023.
//
#include "Signals.h"

void Signals::setup() { signal(SIGINT, handle); }

void Signals::handle(int signum) { shouldQuit = true; }
