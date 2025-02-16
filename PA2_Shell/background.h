//This is the header file for background.c
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <sys/types.h>

// Struct to keep track of background processes (optional)
typedef struct BackgroundProcess {
    pid_t pid;                 // Process ID
    struct BackgroundProcess *next; // Pointer to next process in a linked list
} BackgroundProcess;

// Function declarations
void add_background_process(pid_t pid);
void check_background_processes();
void cleanup_background_process(pid_t pid);

#endif // BACKGROUND_H

