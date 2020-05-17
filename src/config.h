#ifndef CONFIG_H
#define CONFIG_H

#include <termios.h>

struct editor_row {
	int size;
	char *chars;
};

struct editor_config {
	int cx, cy;
	int rows;
	int cols;
	int text_num_rows;
	struct editor_row row;
	struct termios orig_termios;
};

/* Global state variable */
extern struct editor_config EDITOR_CONFIG;

void init_config(void);

#endif
