#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"

#define MAX_ARG_SIZE 64  // Max number of arguments

// Execute a command using fork and execvp
void execute_command(char *cmd) {
    char *args[MAX_ARG_SIZE]; 
    char *token = strtok(cmd, " \t\n"); // Tokenize input

    if (token == NULL) return; // Ignore empty input

    int i = 0;
    while (token != NULL && i < MAX_ARG_SIZE - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL; // Null-terminate argument list

    if (strcmp(args[0], "exit") == 0) {
        if (args[1] != NULL) {
            print_error(); // exit should not have arguments
        } else {
            exit(0);
        }
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        print_error(); // Fork failed
    } else if (pid == 0) {
        execvp(args[0], args);
        print_error(); // If execvp fails
        exit(1);
    } else {
        waitpid(pid, NULL, 0); // Parent waits for child process
    }
}
