//utils.c
/*
 * utils.c - Utility functions -->Created by Cameron Daly & Jed Henry
 * 
 * This file contains helper functions used throughout the shell,
 * such as printing error messages.
 */

#include <ctype.h>
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

/*
 * trim - Removes leading and trailing whitespace from a string.
 * Returns a pointer to the trimmed string.
 * Used this to fix the issues with background processes.
 */
char *trim(char *str) {
    if (str == NULL) return str;
    // Trim leading whitespace
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == '\0') { // All spaces
        return str;
    }
    // Trim trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    return str;
}