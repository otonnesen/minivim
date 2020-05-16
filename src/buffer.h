#ifndef BUFFER_H
#define BUFFER_H

struct str_buf {
	char *buf;
	int len;
};

#define BUF_INIT {NULL, 0}

void str_buf_append(struct str_buf *sb, const char *s, int len);
void str_buf_free(struct str_buf *sb);

#endif
