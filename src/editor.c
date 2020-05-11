/*
 * 'editor.c' implements minivim's core functionality, including drawing
 * and input handling.
 */
#include "editor.h"

#include "buffer.h"
#include "config.h"
#include "util.h"

#include <stdlib.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

/* Handle keypress */
void process_key(void) {
	char c = read_key();

	switch (c) {
	case CTRL_KEY('q'):
		clear_screen();
		exit(0);
		break;
	/* case CTRL_KEY('c'): */
	/* 	refresh_screen(); */
	/* 	break; */
	}
}

/*
 * placeholder for 'void draw(void)' that will be implemented and handle all
 * drawing (eventually).
 */
void draw_tildes_buf(str_buf_t *sb) {
	for (int i = 0; i < EDITOR_CONFIG.rows; i++) {
		str_buf_append(sb, "~\r\n", 3);
		str_buf_append(sb, "\x1b[K", 3);
	}
	str_buf_append(sb, "~", 1);
}

/*
 * 'refresh_screen' takes a pointer to a function that writes to a string
 * buffer and wraps it in escape sequences to clear the screen and return the
 * cursor to the correct spot.
 */
void refresh_screen(void) {
	str_buf_t sb = BUF_INIT;

	str_buf_append(&sb, "\x1b[?25l", 6); /* Hide cursor */
	str_buf_append(&sb, "\x1b[H", 3); /* Reset cursor position */

	draw_tildes_buf(&sb);

	str_buf_append(&sb, "\x1b[H", 3); /* Reset cursor position */
	str_buf_append(&sb, "\x1b[?25h", 6); /* Show cursor */

	write(STDOUT_FILENO, sb.buf, sb.len);
	str_buf_free(&sb);
}
