#ifndef KEYPRESS_H
#define KEYPRESS_H

enum editor_key {
	ARROW_UP = 1000,
	ARROW_DOWN,
	ARROW_RIGHT,
	ARROW_LEFT,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN
};

int read_key(void);

#endif
