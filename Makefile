# Macros
CC = gcc
CFLAGS = -Wall

# Dependency rules for non-file targets
all: shell
clean:
	rm -rf shell *.o

# Dependency rules for file targets
shell: main.o shell.o
	$(CC) $(CFLAGS) main.o shell.o -o shell
main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c
shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c