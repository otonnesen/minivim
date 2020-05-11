#include "terminal.h"

#include "buffer.h"
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

	// Disable raw mode on exit
	atexit(disable_raw_mode);

	struct termios raw = E.orig_termios;
	raw.c_lflag &= ~(ECHO); // Disable echo
	raw.c_lflag &= ~(ICANON); // Disable canonical mode
	raw.c_lflag &= ~(IEXTEN); // Disable input processing
	raw.c_lflag &= ~(ISIG); // Disable quit/interrupt/etc signals
	raw.c_iflag &= ~(IXON); // Disable xon/xoff
	raw.c_iflag &= ~(ICRNL); // Disable CR/NL translation
	raw.c_oflag &= ~(OPOST); // Disable output processing (NL/CR translation)

	// Legacy, maybe not necessary
	raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
	raw.c_cflag |= (CS8);

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

static void draw_tildes_buf(str_buf_t *sb) {
	for (int i = 0; i < E.rows; i++) {
		str_buf_append(sb, "~\r\n", 3);
	}
	str_buf_append(sb, "~", 1);
}

static void clear_screen_buf(str_buf_t *sb) {
	str_buf_append(sb, "\x1b[2J", 4);
	str_buf_append(sb, "\x1b[H", 3);
}

void clear_screen(void) {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

void refresh_screen(void) {
	str_buf_t sb = BUF_INIT;

	clear_screen_buf(&sb);

	draw_tildes_buf(&sb);

	write(STDOUT_FILENO, sb.buf, sb.len);
	str_buf_free(&sb);
}
