/*
 * 'config.c' adds the 'editor_config' struct to hold global state and
 * implements several helper methods to use with it.
 */

#include "config.h"

#include "util.h"

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct editor_config E = { 0 };

static int get_window_size(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		return -1;
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

void kill_config(void)
{
	for (int i = 0; i < E.num_lines; i++) {
		free(E.lines[i].chars);
	}

	free(E.lines);
}

void init_config(void)
{
	if (get_window_size(&E.rows, &E.cols) == -1)
		die("get_window_size");
	atexit(kill_config);
}
