# Macros
CC = gcc
CFLAGS = -Wall

# Dependency rules for non-file targets
all: nicshell
clean:
	rm -rf nicshell *.o

# Dependency rules for file targets
nicshell: main.o shell.o
	$(CC) $(CFLAGS) main.o shell.o -o nicshell
main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c
shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c