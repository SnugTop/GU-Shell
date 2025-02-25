/*
*-->Created by Cameron Daly & Jed Henry
*
 * builtins.c - Implementation of shell built-in commands.
 *
 * This file implements the built-in commands for the shell, including:
 *   - exit: Terminates the shell.
 *   - cd: Changes the current working directory.
 *   - pwd: Prints the current working directory.
 *   - history: Displays a circular list of the last 10 commands.
 *   - kill: Sends a SIGTERM signal to a specified process.
 *   - path: Updates the shell's search path for locating external executables.
 *   - clear: Clears the terminal screen.
 *
 * Additionally, it manages a command history buffer and provides the function 
 * add_to_history() to add new commands to the history.
 * All built-in commands include error checking and, upon encountering an error,
 * print a standard error message using print_error().
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "utils.h"
#include "execute.h"

#define MAX_HISTORY 10
char history[MAX_HISTORY][1024];  // Circular buffer for storing command history
int history_count = 0;            // Current count of commands in the history buffer
extern char *search_paths[MAX_PATHS];  // Array of directories for external command lookup

/*
 * builtin_exit - Terminates the shell.
 *
 * Exits the shell with status 0 if no extra arguments are provided.
 * If any additional arguments are present, an error is printed.
 */
void builtin_exit(char **args) {
    if (args[1] != NULL) {
        print_error();
    } else {
        exit(0);
    }
}

/*
 * builtin_clear - Clears the terminal screen.
 *
 * Uses the system() call to execute the "clear" command.
 */
void builtin_clear(){
    system("clear");
}

/*
 * builtin_cd - Changes the current working directory.
 *
 * Accepts exactly one argument which is the target directory.
 * If no argument or more than one argument is provided, an error is printed.
 * Uses the chdir() system call to change directories.
 */
void builtin_cd(char **args) {
    if (args[1] == NULL || args[2] != NULL) {
        print_error();  // Error: incorrect number of arguments for cd
    } else {
        if (chdir(args[1]) != 0) {
            print_error();  // Error: failed to change directory
        }
    }
}

/*
 * builtin_pwd - Prints the current working directory.
 *
 * Uses getcwd() to retrieve the current directory and prints it.
 * If retrieval fails, an error message is printed.
 */
void builtin_pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        print_error();
    }
}

/*
 * builtin_history - Displays the command history.
 *
 * Prints the last 10 commands stored in the history buffer.
 * If any extra arguments are provided, an error is printed.
 */
void builtin_history(char **args) {
    if (args[1] != NULL) {
        print_error();  // Error: history command takes no arguments
        return;
    }

    for (int i = 0; i < history_count; i++) {
        printf("%d %s", i + 1, history[i]);  // Print command number and command
    }
}

/*
 * add_to_history - Adds a command to the history buffer in FIFO order.
 *
 * Removes any trailing newline from the command.
 * The "history" command itself is not stored to prevent recursion.
 * When the history buffer is full, the oldest command is overwritten.
 */
void add_to_history(char *cmd) {
    // Remove trailing newline if present
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    }

    // Do not store the "history" command itself
    if (strcmp(cmd, "history") == 0) {
        return;
    }
    // Add the command to history or shift the history if full
    if (history_count < MAX_HISTORY) {
        snprintf(history[history_count], sizeof(history[history_count]), "%s\n", cmd);
        history_count++;
    } else {
        // Shift history up when maximum size is reached
        for (int i = 1; i < MAX_HISTORY; i++) {
            strcpy(history[i - 1], history[i]);
        }
        snprintf(history[MAX_HISTORY - 1], sizeof(history[MAX_HISTORY - 1]), "%s\n", cmd);
    }
}

/*
 * builtin_kill - Sends a SIGTERM signal to a specified process.
 *
 * Expects exactly one argument (the PID of the process).
 * Validates the PID and attempts to send a termination signal.
 * If the PID is invalid or the kill operation fails, an error is printed.
 */
void builtin_kill(char **args) {
    if (args[1] == NULL || args[2] != NULL) {
        print_error(); // Error: kill requires exactly one argument =
        return;
    }

    // Convert argument to an integer 
    char *endptr;
    int pid = strtol(args[1], &endptr, 10);

    if (*endptr != '\0' || pid <= 0) {
        print_error();  // Error: invalid PID format
        return;
    }

    // Attempt to send SIGTERM to the specified process
    if (kill(pid, SIGTERM) != 0) {
        print_error();
    } else {
        printf("Process %d terminated\n", pid);
    }
}

/*
 * builtin_path - Updates the shell's search paths for external commands.
 *
 * If no arguments are provided, the search path is set to empty,
 * meaning that only built-in commands will be executed.
 * Otherwise, each argument is treated as a directory and stored in the search_paths array.
 * The existing search path is overwritten.
 */
void builtin_path(char **args) {
    if (args[1] == NULL) {
        search_paths[0] = NULL;  // Empty search path (only built-ins will work)
        return;
    }

    int i;
    for (i = 1; args[i] != NULL && i - 1 < MAX_PATHS; i++) {
        search_paths[i - 1] = strdup(args[i]);  // Duplicate and store each directory
    }
    search_paths[i - 1] = NULL;  // Null-terminate the search path array
}
