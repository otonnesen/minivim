/*
 * minivim - a text editor.
 */

#include "config.h"
#include "editor.h"
#include "terminal.h"

int main(int argc, char *argv[])
{
	enable_raw_mode();
	init_config();
	if (argc >= 2) {
		open_file(argv[1]);
	} else {
		open_editor();
	}

	for (;;) {
		refresh_screen();
		process_key();
	}

	return 0;
}
