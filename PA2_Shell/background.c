// background.c
// This file implements background process management for the shell.
// It maintains a linked list of background processes and provides functions
// to add new background processes, check for terminated processes, and clean them up.

#include "background.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static BackgroundProcess *head = NULL; // Head pointer for the linked list of background processes

// add_background_process - Adds a new process to the background process list.
// @param pid: Process ID of the newly created background process.
void add_background_process(pid_t pid) {
    BackgroundProcess *new_process = (BackgroundProcess *)malloc(sizeof(BackgroundProcess));
    if (!new_process) {
        perror("malloc failed");  // Print error if memory allocation fails
        return;
    }
    new_process->pid = pid;
    new_process->next = head; // Insert at the beginning of the list
    head = new_process;
}

// check_background_processes - Checks and reaps any terminated background processes.
// It uses waitpid() in non-blocking mode (WNOHANG) to determine which background
// processes have completed execution. Upon termination, it prints a message and
// cleans up the process from the list.
void check_background_processes() {
    pid_t pid;
    int status;

    // Continuously check for any terminated background processes
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[Background process %d terminated]\n", pid);
        cleanup_background_process(pid);
    }
}

// cleanup_background_process - Removes a background process from the linked list.
// @param pid: Process ID of the background process to be removed.
// It traverses the list, finds the process with the given PID, removes it,
// and frees the allocated memory.
void cleanup_background_process(pid_t pid) {
    BackgroundProcess *prev = NULL, *curr = head;

    // Traverse the list to locate the process with the specified PID
    while (curr) {
        if (curr->pid == pid) {
            // If it's not the head of the list, adjust the previous node's pointer
            if (prev) {
                prev->next = curr->next;
            } else {
                head = curr->next;
            }
            free(curr);  // Free the memory allocated for the process entry
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
