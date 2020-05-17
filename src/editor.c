/*
 * 'editor.c' implements minivim's core functionality, including drawing
 * and input handling.
 */

#include "editor.h"

#include "buffer.h"
#include "config.h"
#include "keypress.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define MINIVIM_VERSION "0.0.1"

/*
 * Moves cursor with hjkl or arrow keys.
 */
static void move_cursor(int key)
{
	switch (key) {
	case 'h':
	case ARROW_LEFT:
		if (EDITOR_CONFIG.cx - 1 >= 0) {
			EDITOR_CONFIG.cx--;
		}
		break;
	case 'j':
	case ARROW_DOWN:
		if (EDITOR_CONFIG.cy + 1 < EDITOR_CONFIG.rows) {
			EDITOR_CONFIG.cy++;
		}
		break;
	case 'k':
	case ARROW_UP:
		if (EDITOR_CONFIG.cy - 1 >= 0) {
			EDITOR_CONFIG.cy--;
		}
		break;
	case 'l':
	case ARROW_RIGHT:
		if (EDITOR_CONFIG.cx + 1 < EDITOR_CONFIG.cols) {
			EDITOR_CONFIG.cx++;
		}
		break;
	}
}

/* Handle keypress */
void process_key(void)
{
	int c = read_key();

	switch (c) {
	case CTRL_KEY('q'):
		clear_screen();
		exit(0);
		break;
	case 'h':
	case 'j':
	case 'k':
	case 'l':
	case ARROW_LEFT:
	case ARROW_DOWN:
	case ARROW_UP:
	case ARROW_RIGHT:
		move_cursor(c);
		break;
	}
}

/*
 * Writes motd to the current line in sb.
 */
static void draw_motd(struct str_buf *sb)
{
	char motd[80];
	int l = snprintf(motd, sizeof(motd), "Minivim -- version %s",
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
}

/*
 * Writes the contents of the open file to sb if one exists, otherwise writes
 * writes the motd a third of the way down the screen.
 */
static void draw(struct str_buf *sb)
{
	for (int i = 0; i < EDITOR_CONFIG.rows; i++) {
		if (EDITOR_CONFIG.num_lines == 0 && i == EDITOR_CONFIG.rows / 3) {
			draw_motd(sb);
			continue;
		}

		if (i >= EDITOR_CONFIG.num_lines) {
			str_buf_append(sb, "~", 1);
		} else {
			int len = EDITOR_CONFIG.lines[i].size;
			if (len > EDITOR_CONFIG.cols)
				len = EDITOR_CONFIG.cols;
			str_buf_append(sb, EDITOR_CONFIG.lines[i].chars, len);
		}

		str_buf_append(sb, "\x1b[K", 3);
		if (i < EDITOR_CONFIG.rows - 1)
			str_buf_append(sb, "\r\n", 2);
	}
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

	/* draw_tildes_buf(&sb); */
	draw(&sb);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", EDITOR_CONFIG.cy + 1,
			EDITOR_CONFIG.cx + 1);
	str_buf_append(&sb, buf, strlen(buf));	/* Move cursor to position specified in EDITOR_CONFIG */

	str_buf_append(&sb, "\x1b[?25h", 6);	/* Show cursor */

	write(STDOUT_FILENO, sb.buf, sb.len);
	str_buf_free(&sb);
}

/*
 * Anything that needs to be done when no file is specified can go here.
 */
void open_editor(void)
{
}

/*
 * 'append_line' takes a string and its length, and appends it to
 * EDITOR_CONFIG.lines, reallocating as necessary.
 */
static void append_line(char *line, size_t len)
{
	EDITOR_CONFIG.lines = realloc(EDITOR_CONFIG.lines,
			sizeof(struct editor_row) *
			(EDITOR_CONFIG.num_lines + 1));

	int line_num = EDITOR_CONFIG.num_lines;
	EDITOR_CONFIG.lines[line_num].size = len;

	EDITOR_CONFIG.lines[line_num].chars = malloc(len + 1);
	memcpy(EDITOR_CONFIG.lines[line_num].chars, line, len);
	EDITOR_CONFIG.lines[line_num].chars[len] = '\0';

	EDITOR_CONFIG.num_lines++;
}

/*
 * 'open_file' takes a filename string and copies its contents into the
 * editor state struct.
 */
void open_file(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (!fp)
		die("fopen");

	EDITOR_CONFIG.lines = malloc(sizeof(*EDITOR_CONFIG.lines));

	char line[1024];
	for (int i = 0; fgets(line, sizeof(line), fp); i++) {
		int len;
		for (len = 0; line[len] != '\n' && line[len] != '\r' &&
				line[len] != '\0' && len < 1024; len++) ;
		append_line(line, len);
	}

	fclose(fp);
}
