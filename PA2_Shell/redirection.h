//This is the header file for redirection.c
#ifndef REDIRECTION_H
#define REDIRECTION_H

/*
 * redirection.h
 *
 * This header declares functions to handle input/output redirection.
 * The function scans a command’s argument list for the redirection operators
 * '<' (for standard input) and '>' (for standard output) and, if found, opens
 * the specified file (creating/truncating as necessary) and uses dup2() to redirect
 * the appropriate standard file descriptor.
 *
 * Returns 0 on success or -1 on error.
 */

int handle_redirection(char **args);

#endif
