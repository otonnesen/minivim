/*
 * minivim - a text editor.
 */

#include "config.h"
#include "editor.h"
#include "terminal.h"


int main(void) {
	enable_raw_mode();
	init_config();

	for (;;) {
		refresh_screen();
		process_key();
	}

	return 0;
}
