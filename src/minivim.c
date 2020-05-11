/*
 * minivim - a text editor.
 */

#include "config.h"
#include "draw.h"
#include "terminal.h"
#include "util.h"

#include<stdlib.h>

#define CTRL_KEY(k) ((k) & 0x1f)

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

int main(void) {
	enable_raw_mode();
	init_editor();

	for (;;) {
		refresh_screen();
		process_key();
	}

	return 0;
}
