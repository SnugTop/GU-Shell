CC = gcc
CFLAGS = -Wall -Wextra -g

all: gush

gush: gush.o execute.o utils.o
	$(CC) $(CFLAGS) -o gush gush.o execute.o utils.o

gush.o: gush.c execute.h utils.h
	$(CC) $(CFLAGS) -c gush.c

execute.o: execute.c execute.h utils.h
	$(CC) $(CFLAGS) -c execute.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o gush
