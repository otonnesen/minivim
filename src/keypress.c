#include "keypress.h"

#include "terminal.h"
#include "util.h"

#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

/* Read and return one keypress */
static char read_key(void) {
	ssize_t nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}

	return c;
}

/* Handle keypress */
void process_key(void) {
	char c = read_key();

	switch (c) {
	case CTRL_KEY('q'):
		clear_screen();
		exit(0);
		break;
	case CTRL_KEY('c'):
		refresh_screen();
		break;
	}
}
