CC=gcc                              # překladač jazyka C
CFLAGS=-std=c99 -Wall -Wextra -Werror  # parametry překladače

proj1: proj1.c
	$(CC) $(CFLAGS) proj1.c -o proj1                                  
