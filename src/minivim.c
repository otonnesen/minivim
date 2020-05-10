/* minivim - a text editor */

#include "raw.h"

#include<ctype.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(void) {
	enable_raw_mode();
	char c;
	for (;;) {
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("read");

		if (iscntrl(c))
			printf("%d\r\n", c);
		else
			printf("%c\r\n", c);

		if (c == 'q')
			break;
	}

	return 0;
}
