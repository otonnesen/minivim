#include "util.h"

#include<stdio.h>
#include<stdlib.h>

/* Error handler */
void die(const char *s) {
	perror(s);
	exit(1);
}
