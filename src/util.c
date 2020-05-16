/*
 * 'util.c' provides various helper functions.
 */

#include "util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Clears screen and moves cursor to top right. */
void clear_screen(void)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

/* Error handler */
void die(const char *s)
{
	clear_screen();

	perror(s);
	exit(1);
}

/* Read and return one keypress */
char read_key(void)
{
	ssize_t nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}

	return c;
}
