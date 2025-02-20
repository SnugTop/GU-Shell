/*
 * execute.c - Command execution logic
 * Checks if a command is built-in; if not, it handles pipes or redirection,
 * and finally executes external commands using execve().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "builtins.h"
#include "utils.h"
#include "redirection.h"
#include "pipes.h"

#define MAX_ARG_SIZE 64
#define MAX_PATHS 10

char *search_paths[MAX_PATHS] = {"/bin", "/usr/bin", NULL}; // Default search path

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

    // Search through the defined search_paths
    for (int i = 0; search_paths[i] != NULL; i++) {
        snprintf(full_path, sizeof(full_path), "%s/%s", search_paths[i], cmd);
        if (access(full_path, X_OK) == 0) {  // Check if file is executable
            return full_path;
        }
    }

    return NULL; // Command not found in search_paths
}

/*
 * execute_command - Processes and executes a command.
 * It checks for pipes, handles history recall, stores the command in history,
 * and then forks. In the child process, it processes redirection and executes
 * external commands using execve(). Built-in commands are handled in the parent.
 */
void execute_command(char *cmd) {
    // Step 1: Handle history recall BEFORE splitting commands
    if (cmd[0] == '!' && cmd[1] != '\0') {
        int index = atoi(cmd + 1);
        if (index <= 0 || index > history_count) {
            print_error();
            return;
        }
        strcpy(cmd, history[index - 1]);  // Replace cmd with history command
        printf("%s\n", cmd);  // Print executed command
    }

    // Step 2: Store the FULL command in history before modifying it
    add_to_history(cmd);

    char *commands[MAX_ARG_SIZE];  // Array to store commands split by '&'
    int cmd_count = 0;

    // Step 3: Split command by '&' for background execution
    char *token = strtok(cmd, "&");
    while (token != NULL && cmd_count < MAX_ARG_SIZE - 1) {
        commands[cmd_count++] = token;
        token = strtok(NULL, "&");
    }
    commands[cmd_count] = NULL;

    // Step 4: Process each split command separately
    for (int j = 0; j < cmd_count; j++) {
        char *single_cmd = commands[j];
        while (*single_cmd == ' ') single_cmd++;  // Trim leading spaces

        // Step 5: Parse the command into arguments.
        char *args[MAX_ARG_SIZE];
        int i = 0;
        token = strtok(single_cmd, " \t\n");
        while (token != NULL && i < MAX_ARG_SIZE - 1) {
            args[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;  // Skip empty commands

        // Step 6: Determine if this command should be backgrounded
        int background = (j != cmd_count - 1); // Background if not last command

        // Step 7: Check for built-in commands
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

        // Step 8: Locate the executable for the command.
        char *full_path = find_executable(args[0]);
        if (full_path == NULL) {
            print_error();
            return;
        }

        // Step 9: Fork a new process to execute the command.
        pid_t pid = fork();
        if (pid < 0) {
            print_error();
        } else if (pid == 0) {
            // In the child process, handle redirection.
            if (handle_redirection(args) < 0) {
                exit(1);
            }
            // Execute the external command.
            execve(full_path, args, NULL);
            print_error();
            exit(1);
        } else {
            if (!background) {
                // Parent waits only if not a background process
                waitpid(pid, NULL, 0);
            } else {
                // Background process: Print the PID
                printf("[Background PID: %d] %s\n", pid, args[0]);
            }
        }
        printf("Executing command: %s\n", full_path);
    }
}

