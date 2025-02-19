//This is the header file for pipes.c
#ifndef PIPES_H
#define PIPES_H

/*
 * pipes.h
 *
 * This header declares functions for handling pipes.
 * The function execute_piped_commands() takes a command line that contains
 * one or more '|' symbols, splits it into individual commands, and executes
 * them connected by pipes.
 *
 * Returns 0 on success or -1 on error.
 */

int execute_piped_commands(char *cmd_line);

#endif
