#ifndef __UTILS_CATCHER_H
#define __UTILS_CATCHER_H

#include <signal.h>

#include "cimple/all.h"

void __catch_signal(int sig) {
    Console.error("Something went wrong. Please read the manual.");
    Console.error("If you believe this is a bug, please report it to the author.");
    Console.error("Author: Jacob Lin <hi@jacoblin.cool>");
    exit(EXIT_FAILURE);
}

void setup_catcher() {
    struct sigaction action;
    action.sa_handler = __catch_signal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGBUS, &action, NULL);
}

#endif  // __UTILS_CATCHER_H
