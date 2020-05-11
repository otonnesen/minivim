#ifndef CONFIG_H
#define CONFIG_H

#include<termios.h>

struct editor_config {
	int rows;
	int cols;
	struct termios orig_termios;
};

/* Global state variable */
extern struct editor_config E;

void init_editor(void);

#endif
