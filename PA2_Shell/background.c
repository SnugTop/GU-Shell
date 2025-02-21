//background.c
//This file is for implentation of background execution (&)
#include "background.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static BackgroundProcess *head = NULL; // Head of the linked list

// Add a new background process to the list
void add_background_process(pid_t pid) {
    BackgroundProcess *new_process = (BackgroundProcess *)malloc(sizeof(BackgroundProcess));
    if (!new_process) {
        perror("malloc failed");
        return;
    }
    new_process->pid = pid;
    new_process->next = head;
    head = new_process;
}

// Check if background processes have completed
void check_background_processes() {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[Background process %d terminated]\n", pid);
        cleanup_background_process(pid);
    }
}

// Remove a completed process from the list
void cleanup_background_process(pid_t pid) {
    BackgroundProcess *prev = NULL, *curr = head;

    while (curr) {
        if (curr->pid == pid) {
            if (prev) {
                prev->next = curr->next;
            } else {
                head = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}                                                                


