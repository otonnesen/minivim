#include "util.h"

#include "terminal.h"

#include<stdio.h>
#include<stdlib.h>

/* Error handler */
void die(const char *s) {
	clear_screen();

	perror(s);
	exit(1);
}
