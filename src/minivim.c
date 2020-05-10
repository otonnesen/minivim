#include "raw.c"

#include<stdio.h>

int main() {
	enable_raw_mode();
	char c;
	for (;;) {
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
			die("read");
		}

		if (iscntrl(c)) {
			printf("%d\r\n", c);
		} else {
			printf("%c\r\n", c);
		}

		if (c == 'q') {
			break;
		}
	}
	disable_raw_mode();

	return 0;
}
