CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb 
snake : snake.c 
	$(CC) $(CFLAGS) -o snake snake.c -lncurses
