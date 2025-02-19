//This file will be for input and output redirection
/*
 * redirection.c
 *
 * Implements input/output redirection for the shell.
 * It scans the passed-in arguments for the symbols '<' and '>',
 * opens the specified files accordingly, and uses dup2() to redirect
 * stdin and/or stdout.
 *
 * If multiple redirection operators or multiple filenames are detected, it
 * prints an error.
 */

#include "redirection.h"
#include "utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int handle_redirection(char **args) {
    int i = 0;
    int redirect_in = 0, redirect_out = 0;
    char *infile = NULL, *outfile = NULL;
    
    // Loop through the argument list to search for redirection operators
    while (args[i] != NULL) {
        if (strcmp(args[i], "<") == 0) {
            // Must have exactly one filename following the '<'
            if (args[i + 1] == NULL || redirect_in) {
                print_error();
                return -1;
            }
            redirect_in = 1;
            infile = args[i + 1];
            // Remove the operator and filename from the args list
            int j = i;
            while (args[j + 2] != NULL) {
                args[j] = args[j + 2];
                j++;
            }
            args[j] = NULL;
            args[j + 1] = NULL;
            continue;  // Recheck current index after shifting
        }
        else if (strcmp(args[i], ">") == 0) {
            // Must have exactly one filename following the '>'
            if (args[i + 1] == NULL || redirect_out) {
                print_error();
                return -1;
            }
            redirect_out = 1;
            outfile = args[i + 1];
            // Remove the operator and filename from the args list
            int j = i;
            while (args[j + 2] != NULL) {
                args[j] = args[j + 2];
                j++;
            }
            args[j] = NULL;
            args[j + 1] = NULL;
            continue;
        }
        i++;
    }
    
    // If input redirection is requested, open the file for reading.
    if (redirect_in) {
        int fd_in = open(infile, O_RDONLY);
        if (fd_in < 0) {
            print_error();
            return -1;
        }
        if (dup2(fd_in, STDIN_FILENO) < 0) { // Redirect stdin to the file
            print_error();
            close(fd_in);
            return -1;
        }
        close(fd_in);
    }
    
    // If output redirection is requested, open (or create) the file for writing.
    if (redirect_out) {
        int fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0) {
            print_error();
            return -1;
        }
        if (dup2(fd_out, STDOUT_FILENO) < 0) {
            print_error();
            close(fd_out);
            return -1;
        }
        close(fd_out);
    }
    return 0;
}
