// background.h
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <sys/types.h>

// BackgroundProcess structure:
// Maintains information for a background process, including its process ID
// and a pointer to the next process in the linked list.
typedef struct BackgroundProcess {
    pid_t pid;                           // Process ID of the background process
    struct BackgroundProcess *next;      // Pointer to the next process in the list
} BackgroundProcess;

// Function declarations for background process management:
// - add_background_process: Adds a new process to the background process list.
// - check_background_processes: Checks for and reaps terminated background processes.
// - cleanup_background_process: Removes a specific process from the background process list.
void add_background_process(pid_t pid);
void check_background_processes();
void cleanup_background_process(pid_t pid);

#endif // BACKGROUND_H
