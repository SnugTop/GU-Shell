/*
 * pipes.c
 *
 * Implements the piping feature for the shell. The execute_piped_commands()
 * function splits a command line (containing the '|' operator) into separate commands,
 * creates pipes to connect the stdout of one command to the stdin of the next, and
 * forks processes to execute each command.
 *
 * Up to 4 pipes (i.e. 5 commands) are supported.
 */

#include "pipes.h"
#include "utils.h"
#include "execute.h"  // For MAX_ARG_SIZE and find_executable()
#include "redirection.h"  // For handle_redirection()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PIPE_CMDS 5  // Supports up to 4 pipes, i.e. 5 commands

int execute_piped_commands(char *cmd_line) {
    char *cmds[MAX_PIPE_CMDS];
    int num_cmds = 0;

    // Split the command line into commands separated by '|'
    char *token = strtok(cmd_line, "|");
    while (token != NULL && num_cmds < MAX_PIPE_CMDS) {
        cmds[num_cmds++] = token;
        token = strtok(NULL, "|");
    }

    // Trim leading and trailing whitespace from each command
    for (int i = 0; i < num_cmds; i++) {
        // Trim leading spaces
        while (*cmds[i] == ' ') {
            cmds[i]++;
        }
        // Trim trailing whitespace
        char *end = cmds[i] + strlen(cmds[i]) - 1;
        while (end > cmds[i] && (*end == ' ' || *end == '\n')) {
            *end = '\0';
            end--;
        }
    }

    int pipefds[2 * (num_cmds - 1)];
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            print_error();
            return -1;
        }
    }

    int status;
    pid_t pid;
    for (int i = 0; i < num_cmds; i++) {
        pid = fork();
        if (pid < 0) {
            print_error();
            return -1;
        } else if (pid == 0) {  // Child process
            // If not the first command, redirect stdin to previous pipe's read end
            if (i != 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
                    print_error();
                    exit(1);
                }
            }
            // If not the last command, redirect stdout to current pipe's write end
            if (i != num_cmds - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
                    print_error();
                    exit(1);
                }
            }
            // Close all pipe file descriptors in the child process
            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }
            // Parse the individual command into arguments
            char *args[MAX_ARG_SIZE];
            int k = 0;
            char *arg = strtok(cmds[i], " \t\n");
            while (arg != NULL && k < MAX_ARG_SIZE - 1) {
                args[k++] = arg;
                arg = strtok(NULL, " \t\n");
            }
            args[k] = NULL;
            
            // Handle any redirection in the sub-command.
            // This call will scan args for '<' or '>' and set up redirection,
            // removing the redirection tokens from args.
            if (handle_redirection(args) < 0) {
                print_error();
                exit(1);
            }
            
            // Locate the executable for the command.
            char *full_path = find_executable(args[0]);
            if (full_path == NULL) {
                print_error();
                exit(1);
            }
            // Execute the command.
            execve(full_path, args, NULL);
            print_error();
            exit(1);
        }
    }
    
    // Close all pipe file descriptors in the parent process.
    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }
    
    // Wait for all child processes to finish.
    for (int i = 0; i < num_cmds; i++) {
        wait(&status);
    }
    
    return 0;
}
