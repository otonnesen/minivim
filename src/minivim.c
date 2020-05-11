/* minivim - a text editor */

#include "config.h"
#include "terminal.h"
#include "keypress.h"

int main(void) {
	enable_raw_mode();
	init_editor();

	for (;;) {
		process_key();
	}

	return 0;
}
