//Header file for built-in commands builtins.h
/*
 * builtins.h - Header file for built-in commands
 *
 * This file declares functions for built-in commands such as:
 * - exit
 * - cd
 * - pwd
 * - history
 * - kill
 * - path
 * - clear
 */

#ifndef BUILTINS_H
#define BUILTINS_H
#define MAX_HISTORY 10


#include "execute.h"  // Needed for MAX_PATHS and search_paths

extern char history[MAX_HISTORY][1024];  // Store command history
extern int history_count;

// Function prototypes for built-in commands
void builtin_exit(char **args);
void builtin_cd(char **args);
void builtin_pwd();
void builtin_clear();
void builtin_history(char **args);
void builtin_kill(char **args);
void builtin_path(char **args);
void add_to_history(char *cmd);

#endif
