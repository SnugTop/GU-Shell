//utils.c
/*
 * utils.c - Utility functions
 * 
 * This file contains helper functions used throughout the shell,
 * such as printing error messages.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERROR_MSG "An error has occurred\n"

/*
 * print_error - Prints a standard error message to stderr
 */
void print_error() {
    write(STDERR_FILENO, ERROR_MSG, strlen(ERROR_MSG));
}
