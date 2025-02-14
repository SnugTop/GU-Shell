#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execute.h"
#include "utils.h"

#define MAX_INPUT_SIZE 1024  // Max command length

// Interactive mode
void interactive_mode() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        printf("gush> "); // Print prompt
        fflush(stdout);
        
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            exit(0); // Exit on EOF (Ctrl+D)
        }

        execute_command(input);
    }
}

// Batch mode
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

int main(int argc, char *argv[]) {
    if (argc > 2) {
        print_error();
        exit(1);
    }

    if (argc == 2) {
        batch_mode(argv[1]); // Run batch mode
    } else {
        interactive_mode(); // Run interactive mode
    }

    return 0;
}
