/*
 * 'buffer.c' implements a dynamically allocated string buffer for use
 * when writing to the terminal.
 */

#include "buffer.h"

#include <stdlib.h>
#include <string.h>

void str_buf_append(str_buf_t *sb, const char *s, int len) {
	char *new = realloc(sb->buf, sb->len + len);

	if (new == NULL)
		return;

	memcpy(&new[sb->len], s, len);
	sb->buf = new;
	sb->len += len;
}

void str_buf_free(str_buf_t *sb) {
	free(sb->buf);
}
