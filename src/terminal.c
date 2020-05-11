/*
 * 'terminal.c' handles updating the low-level terminal settings.
 */

#include "terminal.h"

#include "config.h"
#include "util.h"

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

/* Resets terminal to default */
static void disable_raw_mode(void) {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
		die("tcsetattr");
}

/* Sets terminal to non-canonical mode */
void enable_raw_mode(void) {
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)
		die("tcgetattr");

	/* Disable raw mode on exit */
	atexit(disable_raw_mode);

	struct termios raw = E.orig_termios;
	/*
	 * Disable echo, canonical mode, input processing, and quit/interrupt
	 * signal processing.
	 */
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

	/* Disable xon/xoff, CR/NL translation */
	raw.c_iflag &= ~(IXON | ICRNL);

	/* Disable output processing (NL/CR translation) */
	raw.c_oflag &= ~(OPOST);

	/* Legacy, maybe not necessary */
	raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
	raw.c_cflag |= (CS8);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}
