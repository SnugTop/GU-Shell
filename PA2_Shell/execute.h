/*
 * execute.h - Header file for execute.c
 * 
 * This header file contains function prototypes for command execution.
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#define MAX_PATHS 10  // Define MAX_PATHS globally
#define MAX_ARG_SIZE 64  // Define MAX_ARG_SIZE globally

extern char *search_paths[MAX_PATHS]; // Declare search_paths globally

void execute_command(char *cmd);
char *find_executable(char *cmd);

#endif
