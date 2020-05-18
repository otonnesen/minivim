#ifndef CONFIG_H
#define CONFIG_H

#include <termios.h>

struct editor_row {
	int size;
	char *chars;
};

struct editor_config {
	int cx, cy;
	int row_offset;
	int col_offset;
	int rows;
	int cols;
	int num_lines;
	struct editor_row *lines;
	struct termios orig_termios;
};

/* Global state variable */
extern struct editor_config E;

void init_config(void);

#endif
