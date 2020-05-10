#include<ctype.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>

/* Error handler */
void die(const char *s) {
	perror(s);
	exit(1);
} 

/* Default terminal attributes */
struct termios orig_termios;

/* Resets terminal to default */
void disable_raw_mode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
		die("tcsetattr");
	}
}

/* Sets terminal to non-canonical mode */
void enable_raw_mode() {
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
		die("tcgetattr");
	}
	// Disable raw mode on exit
	atexit(disable_raw_mode);

	struct termios raw = orig_termios;
	// Disable echo
	raw.c_lflag &= ~(ECHO);
	// Disable canonical mode
	raw.c_lflag &= ~(ICANON);
	// Disable input processing
	raw.c_lflag &= ~(IEXTEN);
	// Disable quit/interrupt/etc signals
	raw.c_lflag &= ~(ISIG);
	// Disable xon/xoff
	raw.c_iflag &= ~(IXON);
	// Disable CR/NL translation
	raw.c_iflag &= ~(ICRNL);
	raw.c_oflag &= ~(OPOST);

	// Legacy, maybe not necessary
	raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
	raw.c_cflag |= (CS8);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
		die("tcsetattr");
	}
}
