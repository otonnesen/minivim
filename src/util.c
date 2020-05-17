/*
 * 'util.c' provides various helper functions.
 */

#include "util.h"

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
