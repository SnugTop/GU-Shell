//backgroun.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <sys/types.h>

// Struct to keep track of background processes
typedef struct BackgroundProcess {
    pid_t pid;
    struct BackgroundProcess *next;
} BackgroundProcess;

// Function declarations for background processing
void add_background_process(pid_t pid);
void check_background_processes();
void cleanup_background_process(pid_t pid);

#endif // BACKGROUND_H
