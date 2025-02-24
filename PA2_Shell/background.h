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
void add_background_process(pid_t pid);
void check_background_processes();
void cleanup_background_process(pid_t pid);

#endif 