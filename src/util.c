/*
 * 'util.c' provides various helper functions.
 */

#include "util.h"

#include "draw.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Error handler */
void die(const char *s) {
	clear_screen();

	perror(s);
	exit(1);
}

/* Read and return one keypress */
char read_key(void) {
	ssize_t nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}

	return c;
}

