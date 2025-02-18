#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "utils.h"
#include "execute.h"

#define MAX_HISTORY 10
char history[MAX_HISTORY][1024];
int history_count = 0;
extern char *search_paths[MAX_PATHS];

/*
 * builtin_exit - Exits the shell
 */
void builtin_exit(char **args) {
    if (args[1] != NULL) {
        print_error();
    } else {
        exit(0);
    }
}

/*builtin_clear - Clears the terminal for a cleaner workspace
*/

void builtin_clear(){
    system("clear");
}

/*
 * builtin_cd - Changes directory without using execve()
 */
void builtin_cd(char **args) {
    if (args[1] == NULL || args[2] != NULL) {
        print_error();  // `cd` must have exactly one argument
    } else {
        if (chdir(args[1]) != 0) {
            print_error();  // Error: invalid directory
        }
    }
}

/*
 * builtin_pwd - Prints the current directory without using execve()
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
 * builtin_history - Prints the last 10 commands entered
 */
void builtin_history(char **args) {
    if (args[1] != NULL) {
        print_error();  // `history` takes no arguments
        return;
    }

    for (int i = 0; i < history_count; i++) {
        printf("%d %s", i + 1, history[i]);  // Print history
    }
}

/*
 * add_to_history - Stores command into history (FIFO order)
 */
 void add_to_history(char *cmd) {
    // Remove trailing newline if present
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    }

    // Ignore "history" command to prevent it from being stored
    if (strcmp(cmd, "history") == 0) {
        return;
    }
 // Store the command, ensuring it's followed by a newline
    if (history_count < MAX_HISTORY) {
        snprintf(history[history_count], sizeof(history[history_count]), "%s\n", cmd);
        history_count++;
    } else {
        // Shift history up (FIFO) when max size reached
        for (int i = 1; i < MAX_HISTORY; i++) {
            strcpy(history[i - 1], history[i]);
        }
        snprintf(history[MAX_HISTORY - 1], sizeof(history[MAX_HISTORY - 1]), "%s\n", cmd);
    }
}

/*
 * builtin_kill - Sends a signal to a process
 */
void builtin_kill(char **args) {
    if (args[1] == NULL || args[2] != NULL) {
        print_error(); // Kill requires exactly one argument (PID)
        return;
    }

    // Convert argument to an integer (PID)
    char *endptr;
    int pid = strtol(args[1], &endptr, 10);

    if (*endptr != '\0' || pid <= 0) {
        print_error();  // Invalid PID
        return;
    }

    // Try to send SIGTERM signal
    if (kill(pid, SIGTERM) != 0) {
        print_error();
    } else {
        printf("Process %d terminated\n", pid);
    }
}

/*
 * builtin_path - Updates the search paths manually
 */
void builtin_path(char **args) {
    if (args[1] == NULL) {
        search_paths[0] = NULL;  // Empty path means only built-ins will work
        return;
    }

    int i;
    for (i = 1; args[i] != NULL && i - 1 < MAX_PATHS; i++) {
        search_paths[i - 1] = strdup(args[i]);  // Store each directory in the search path
    }
    search_paths[i - 1] = NULL;  // Null-terminate the path array
}
