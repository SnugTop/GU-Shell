/*
 * execute.c - Command execution logic
 * Checks if a command is built-in; otherwise, runs it using execve().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "builtins.h"
#include "utils.h"

#define MAX_ARG_SIZE 64
#define MAX_PATHS 10

char *search_paths[MAX_PATHS] = {"/bin", NULL}; // Default search path

/*
 * find_executable - Resolves a command to its full path by checking search paths.
 */
char *find_executable(char *cmd) {
    static char full_path[1024];

    // If the command already has an absolute or relative path, use it directly
    if (cmd[0] == '/' || cmd[0] == '.') {
        return cmd;
    }

    // If path is empty, return NULL (no external commands should execute)
    if (search_paths[0] == NULL) {
        return NULL;
    }

    // Search through the defined `path`
    for (int i = 0; search_paths[i] != NULL; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", search_paths[i], cmd);
        if (access(full_path, X_OK) == 0) {  // Check if file is executable
            return full_path;
        }
    }

    return NULL; // Command not found in `path`
}

/*
 * execute_command - Runs a command by checking built-ins first, then execve().
 */
void execute_command(char *cmd) {
    add_to_history(cmd);  // Add command to history
    char *args[MAX_ARG_SIZE];
    char *token = strtok(cmd, " \t\n");

    if (token == NULL) return;

    int i = 0;
    while (token != NULL && i < MAX_ARG_SIZE - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;

    // Check for built-in commands FIRST before calling execve()
    if (strcmp(args[0], "exit") == 0) {
        builtin_exit(args);
        return;
    }
    if (strcmp(args[0], "cd") == 0) {
        builtin_cd(args);
        return;
    }
    if (strcmp(args[0], "pwd") == 0) {
        builtin_pwd();
        return;
    }
    if (strcmp(args[0], "history") == 0) {
        builtin_history(args);
        return;
    }
    if (strcmp(args[0], "kill") == 0) {
        builtin_kill(args);
        return;
    }
    if (strcmp(args[0], "path") == 0) {
        builtin_path(args);
        return;
    }
    if (strcmp(args[0], "clear") == 0) {  
        builtin_clear();  
        return;
    }

    // If it's not a built-in, manually search for the command
    char *full_path = find_executable(args[0]);
    if (full_path == NULL) {
        print_error();
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        print_error();
    } else if (pid == 0) {
        execve(full_path, args, NULL);  // Run external command
        print_error();
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
    printf("Executing command: %s\n", full_path);
}
