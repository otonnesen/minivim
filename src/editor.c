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

static void scroll(int vert, int hor)
{
	E.row_offset += vert;

	if (E.row_offset < 0)
		E.row_offset = 0;
	else if (E.row_offset >= E.num_lines)
		E.row_offset = E.num_lines - 1;

	E.col_offset += hor;

	if (E.col_offset < 0)
		E.col_offset = 0;
	else if (E.col_offset >= E.lines[E.cy + E.row_offset].size
			&& E.lines[E.cy + E.row_offset].size > 0)
		E.col_offset = E.lines[E.cy + E.row_offset].size - 1;
}

static void fix_cursor(void)
{
	if (E.cy < 0) {
		scroll(-1, 0);
		E.cy = 0;
	} else if (E.cy >= E.num_lines) {
		scroll(1, 0);
		E.cy = E.num_lines - 1;
	}

	if (E.cx < 0) {
		scroll(0, -1);
		E.cx = 0;
	} else if (E.cx + E.col_offset >= E.lines[E.cy + E.row_offset].size) {
		E.cx = E.lines[E.cy + E.row_offset].size - E.col_offset - 1;
		/* TODO: Make tabs count as 8 characters instead of 1 */
	} else if (E.cx >= E.cols) {
		scroll(0, 1);
		E.cx = E.cols - 1;
	}
}

/*
 * Moves the cursor with hjkl or arrow keys. Changes E.row_offset
 * if the cursor moves off the screen.
 */
static void move_cursor(int key)
{
	switch (key) {
	case 'h':
	case ARROW_LEFT:
		E.cx--;
		break;
	case 'j':
	case ARROW_DOWN:
		E.cy++;
		break;
	case 'k':
	case ARROW_UP:
		E.cy--;
		break;
	case 'l':
	case ARROW_RIGHT:
		E.cx++;
		break;
	}

	fix_cursor();
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
	case CTRL_KEY('e'):
		scroll(1, 0);
		break;
	case CTRL_KEY('y'):
		scroll(-1, 0);
		break;
	case CTRL_KEY('d'):
		scroll(E.rows / 2, 0);
		break;
	case CTRL_KEY('u'):
		scroll(-1 * E.rows / 2, 0);
		break;
	case 'H':
		E.cy = 0;
		break;
	case 'M':
		E.cy = E.rows / 2;
		break;
	case 'L':
		E.cy = E.rows - 1;
		break;
	case '^':
		E.cx = 0;
		break;
	case '$':
		E.cx = E.cols - 1;
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
	if (l > E.cols)
		l = E.cols;
	int p = (E.cols - l) / 2;
	str_buf_append(sb, "~", 1);
	p--;
	while (p--)
		str_buf_append(sb, " ", 1);
	str_buf_append(sb, motd, l);
	str_buf_append(sb, "\x1b[K", 3);
	str_buf_append(sb, "\r\n", 2);
}

static void draw_debug(struct str_buf *sb)
{
	char msg[80];
	int linelen = E.lines[E.cy + E.row_offset].size;
	int l = snprintf(msg, sizeof(msg),
			"DEBUG: linelen: %d, E.cx: %d, E.cols: %d, E.col_offset: %d",
			linelen, E.cx, E.cols, E.col_offset);
	if (l > E.cols)
		l = E.cols;
	str_buf_append(sb, msg, l);
	str_buf_append(sb, "\x1b[K", 3);
}

/*
 * Writes the contents of the open file to sb if one exists, otherwise writes
 * the motd a third of the way down the screen.
 */
static void draw(struct str_buf *sb)
{
	for (int i = 0; i < E.rows; i++) {
		str_buf_append(sb, "\x1b[2K", 4);
		int text_line = i + E.row_offset;
		if (E.num_lines == 0 && i == E.rows / 3) {
			draw_motd(sb);
			continue;
		}

		if (i == E.rows - 1) {
			draw_debug(sb);
			continue;
		}

		if (text_line >= E.num_lines) {
			str_buf_append(sb, "~", 1);
		} else {
			int len = E.lines[text_line].size - E.col_offset;
			if (len > E.cols)
				len = E.cols;
			str_buf_append(sb, E.lines[text_line]
					.chars + E.col_offset, len);
			/* TODO: Wrap lines instead of truncating. */
		}

		str_buf_append(sb, "\x1b[K", 3);
		if (i < E.rows - 1)
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

	draw(&sb);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
	str_buf_append(&sb, buf, strlen(buf));	/* Move cursor to position specified in E */

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
 * E.lines, reallocating as necessary.
 */
static void append_line(char *line, size_t len)
{
	E.lines = realloc(E.lines,
			sizeof(struct editor_row) * (E.num_lines + 1));

	int line_num = E.num_lines;
	E.lines[line_num].size = len;

	E.lines[line_num].chars = malloc(len + 1);
	memcpy(E.lines[line_num].chars, line, len);
	E.lines[line_num].chars[len] = '\0';

	E.num_lines++;
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

	E.lines = malloc(sizeof(*E.lines));

	char line[1024];
	for (int i = 0; fgets(line, sizeof(line), fp); i++) {
		int len;
		for (len = 0; line[len] != '\n' && line[len] != '\r' &&
				line[len] != '\0' && len < 1024; len++) ;
		append_line(line, len);
	}

	fclose(fp);
}
