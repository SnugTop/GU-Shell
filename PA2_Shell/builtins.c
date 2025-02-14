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
 * builtin_history - Prints the command history
 */
void builtin_history(char **args) {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s", i + 1, history[i]);
    }
}

/*
 * builtin_kill - Sends a signal to a process
 */
void builtin_kill(char **args) {
    if (args[1] == NULL || args[2] != NULL) {
        print_error();
        return;
    }
    int pid = atoi(args[1]);
    if (pid <= 0 || kill(pid, SIGTERM) != 0) {
        print_error();
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
