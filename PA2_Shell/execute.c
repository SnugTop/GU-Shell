/*
 * execute.c - Command execution logic
 * 
 * This file is responsible for executing user commands.
 * It uses fork() to create child processes and execvp() to run the command.
 * Built-in commands (like "exit") are handled separately.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"

#define MAX_ARG_SIZE 64  // Maximum number of arguments

/*
 * execute_command - Runs a command entered by the user
 * Uses fork() to create a child process and execvp() to execute the command.
 */
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

    // Check for built-in "exit" command
    if (strcmp(args[0], "exit") == 0) {
        if (args[1] != NULL) {
            print_error(); // exit should not have arguments
        } else {
            exit(0);
        }
        return;
    }

    pid_t pid = fork(); // Create a child process
    if (pid < 0) {
        print_error(); // Fork failed
    } else if (pid == 0) { // Child process
        execvp(args[0], args);
        print_error(); // If execvp fails, print error
        exit(1);
    } else { // Parent process
        waitpid(pid, NULL, 0); // Wait for child process to complete
    }
}
