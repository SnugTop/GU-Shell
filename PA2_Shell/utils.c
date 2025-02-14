#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERROR_MSG "An error has occurred\n"

void print_error() {
    write(STDERR_FILENO, ERROR_MSG, strlen(ERROR_MSG));
}
