#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

void my_shell() {
    char input[MAX_INPUT];
    char *args[100];  // Argument list
    int status;

    while (1) {
        printf("gush> ");  // Shell prompt
        fgets(input, MAX_INPUT, stdin);

        // Remove newline character
        input[strcspn(input, "\n")] = 0;

        // Exit shell
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Tokenize input
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");
        }

        // Fork process
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            if (execvp(args[0], args) < 0) {
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            waitpid(pid, &status, 0);
        }
    }
}

int main() {
    my_shell();
    return 0;
}