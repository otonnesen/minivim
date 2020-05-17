/*
 * 'config.c' adds the 'editor_config' struct to hold global state and
 * implements several helper methods to use with it.
 */

#include "config.h"

#include "util.h"

#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct editor_config EDITOR_CONFIG = { 0 };

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
	for (int i = 0; i < EDITOR_CONFIG.num_lines; i++) {
		free(EDITOR_CONFIG.lines[i].chars);
	}

	free(EDITOR_CONFIG.lines);
}

void init_config(void)
{
	if (get_window_size(&EDITOR_CONFIG.rows, &EDITOR_CONFIG.cols) == -1)
		die("get_window_size");
	atexit(kill_config);
}
