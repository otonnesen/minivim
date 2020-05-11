/*
 * 'draw.c' implements functions relating to clearing and writing to the
 * terminal.
 */

#include "draw.h"

#include "buffer.h"
#include "config.h"

#include <unistd.h>

static void draw_tildes_buf(str_buf_t *sb) {
	for (int i = 0; i < E.rows; i++) {
		str_buf_append(sb, "~\r\n", 3);
		str_buf_append(sb, "\x1b[K", 3);
	}
	str_buf_append(sb, "~", 1);
}

void clear_screen(void) {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

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
