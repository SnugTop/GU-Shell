/*
 * gush.c - Main shell logic
 * 
 * This file contains the main function that initializes the shell.
 * It supports both interactive mode (where the user types commands) and
 * batch mode (where commands are read from a file). 
 * The main function determines which mode to run and processes user commands.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execute.h"
#include "utils.h"

#define MAX_INPUT_SIZE 1024  // Maximum command length

/*
 * interactive_mode - Runs the shell in interactive mode
 * This function continuously prompts the user for input, reads the command,
 * and sends it for execution.
 */
void interactive_mode() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("gush> "); // Print shell prompt
        fflush(stdout);
        
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            exit(0); // Exit on EOF (Ctrl+D)
        }

        execute_command(input); // Process the input command
    }
}

/*
 * batch_mode - Runs the shell in batch mode
 * This function reads commands from a given file and executes them.
 */
void batch_mode(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        print_error();  // Print error if file cannot be opened
        exit(1);
    }

    char input[MAX_INPUT_SIZE];
    while (fgets(input, MAX_INPUT_SIZE, file)) {
        execute_command(input); // Execute each command from the file
    }
    
    fclose(file);
    exit(0);
}

/*
 * main - Entry point of the shell
 * Determines whether the shell runs in interactive or batch mode.
 */
int main(int argc, char *argv[]) {
    if (argc > 2) {
        print_error(); // Too many arguments (invalid usage)
        exit(1);
    }

    if (argc == 2) {
        batch_mode(argv[1]); // Run batch mode with input file
    } else {
        interactive_mode(); // Run interactive mode
    }

    return 0;
}
