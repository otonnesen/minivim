/* minivim - a text editor */

#include "terminal.h"
#include "keypress.h"

#include<stdio.h>

int main(void) {
	enable_raw_mode();
	for (;;) {
		process_key();
	}

	return 0;
}
