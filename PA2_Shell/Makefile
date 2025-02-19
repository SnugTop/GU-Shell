# Makefile for compiling the Gush shell

CC = gcc
CFLAGS = -Wall -Wextra -g

# Default target: Compile the shell
all: gush

# Build the final executable
gush: gush.o execute.o builtins.o utils.o background.o pipes.o redirection.o
	$(CC) $(CFLAGS) -o gush gush.o execute.o builtins.o utils.o background.o pipes.o redirection.o

# Compile individual object files
gush.o: gush.c execute.h builtins.h utils.h background.h pipes.h redirection.h
	$(CC) $(CFLAGS) -c gush.c

execute.o: execute.c execute.h builtins.h utils.h
	$(CC) $(CFLAGS) -c execute.c

builtins.o: builtins.c execute.h builtins.h utils.h
	$(CC) $(CFLAGS) -c builtins.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

background.o: background.c background.h
	$(CC) $(CFLAGS) -c background.c

pipes.o: pipes.c pipes.h
	$(CC) $(CFLAGS) -c pipes.c

redirection.o: redirection.c redirection.h
	$(CC) $(CFLAGS) -c redirection.c

# Clean compiled files
clean:
	rm -f *.o gush
