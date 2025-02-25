//gush.c-->Created by Cameron Daly & Jed Henry
/*
 * gush.c - Main shell logic (Final Edition)
 * Handles interactive mode (with prompt) and batch mode (reading from a file).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execute.h"
#include "builtins.h"
#include "utils.h"

#define MAX_INPUT_SIZE 1024  // Maximum command length

/*
 * interactive_mode - Runs the shell in interactive mode.
 */
void interactive_mode() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("gush> ");
        fflush(stdout);
        
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            exit(0);
        }

        execute_command(input);
    }
}

/*
 * batch_mode - Runs the shell in batch mode.
 */
void batch_mode(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        print_error();
        exit(1);
    }

    char input[MAX_INPUT_SIZE];
    while (fgets(input, MAX_INPUT_SIZE, file)) {
        execute_command(input);
    }
    
    fclose(file);
    exit(0);
}

/*
 * main - Entry point of the shell.
 */
int main(int argc, char *argv[]) {
    if (argc > 2) {
        print_error();
        exit(1);
    }

    if (argc == 2) {
        batch_mode(argv[1]); 
    } else {
        interactive_mode(); 
    }

    return 0;
}
