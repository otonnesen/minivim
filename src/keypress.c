/*
 * 'keypress.c' primarily implements the 'read_key' function that reads and
 * returns the user's input.
 */

#include "keypress.h"

#include "util.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

static int parse_control_sequence(void)
{
	char seq[3];

	if (read(STDIN_FILENO, &seq[0], 1) != 1)
		return '\x1b';
	if (read(STDIN_FILENO, &seq[1], 1) != 1)
		return '\x1b';

	if (seq[0] == '[') {
		if (seq[1] >= '0' && seq[1] <= '9') {
			if (read(STDIN_FILENO, &seq[2], 1) != 1)
				return '\x1b';
			if (seq[2] == '~') {
				switch (seq[1]) {
				case '1':
					return HOME_KEY;
				case '4':
					return END_KEY;
				case '5':
					return PAGE_UP;
				case '6':
					return PAGE_DOWN;
				case '7':
					return HOME_KEY;
				case '8':
					return END_KEY;
				}
			}
		} else {
			switch (seq[1]) {
			case 'A':
				return ARROW_UP;
			case 'B':
				return ARROW_DOWN;
			case 'C':
				return ARROW_RIGHT;
			case 'D':
				return ARROW_LEFT;
			}
		}
	} else if (seq[0] == 'O') {
		switch (seq[1]) {
		case 'H':
			return HOME_KEY;
		case 'F':
			return END_KEY;
		}
	}
	return '\x1b';
}

/* Read and return one keypress */
int read_key(void)
{
	ssize_t nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}

	/* Parse control sequence */
	if (c == '\x1b') {
		return parse_control_sequence();
	} else {
		return c;
	}

	return c;
}
