/*
 * 'editor.c' implements minivim's core functionality, including drawing
 * and input handling.
 */
#include "editor.h"

#include "buffer.h"
#include "config.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define MINIVIM_VERSION "0.0.1"

/* Handle keypress */
void process_key(void)
{
	char c = read_key();

	switch (c) {
	case CTRL_KEY('q'):
		clear_screen();
		exit(0);
		break;
	}
}

/*
 * placeholder for 'void draw(struct str_buf*)' that will be implemented and handle all
 * drawing (eventually).
 */
void draw_tildes_buf(struct str_buf *sb)
{
	for (int i = 0; i < EDITOR_CONFIG.rows; i++) {
		if (i == EDITOR_CONFIG.rows / 3) {
			char motd[80];
			int l = snprintf(motd, sizeof(motd),
					"Minivim -- version %s",
					MINIVIM_VERSION);
			if (l > EDITOR_CONFIG.cols)
				l = EDITOR_CONFIG.cols;
			int p = (EDITOR_CONFIG.cols - l) / 2;
			str_buf_append(sb, "~", 1);
			p--;
			while (p--)
				str_buf_append(sb, " ", 1);
			str_buf_append(sb, motd, l);
			str_buf_append(sb, "\r\n", 2);
		} else {
			str_buf_append(sb, "~\r\n", 3);
		}
		str_buf_append(sb, "\x1b[K", 3);
	}
	str_buf_append(sb, "~", 1);
}

/*
 * 'refresh_screen' calls the draw function and resets the cursor's position
 * to the top right of the terminal.
 */
void refresh_screen(void)
{
	struct str_buf sb = BUF_INIT;

	str_buf_append(&sb, "\x1b[?25l", 6);	/* Hide cursor */
	str_buf_append(&sb, "\x1b[H", 3);	/* Reset cursor position */

	draw_tildes_buf(&sb);

	str_buf_append(&sb, "\x1b[H", 3);	/* Reset cursor position */
	str_buf_append(&sb, "\x1b[?25h", 6);	/* Show cursor */

	write(STDOUT_FILENO, sb.buf, sb.len);
	str_buf_free(&sb);
}
