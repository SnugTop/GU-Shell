//Header file for built-in commands
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
 */

#ifndef BUILTINS_H
#define BUILTINS_H

#include "execute.h"  // Needed for MAX_PATHS and search_paths

// Function prototypes for built-in commands
void builtin_exit(char **args);
void builtin_cd(char **args);
void builtin_pwd();
void builtin_history(char **args);
void builtin_kill(char **args);
void builtin_path(char **args);

#endif
