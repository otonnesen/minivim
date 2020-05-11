#ifndef BUFFER_H
#define BUFFER_H
typedef struct str_buf {
	char *buf;
	int len;
} str_buf_t;

#define BUF_INIT {NULL, 0}

void str_buf_append(str_buf_t*, const char*, int);
void str_buf_free(str_buf_t*);

#endif
