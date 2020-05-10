CC = gcc
CFLAGS = -O -Wall -Wextra -pedantic -std=c99

TARGETS = minivim terminal keypress util

SRC = $(patsubst %,obj/%.o,$(TARGETS))

all: compile

obj/%.o: src/%.c | obj
	$(CC) -c -o $@ $< $(CFLAGS)

obj:
	mkdir -p $@

.PHONY: compile

compile: $(SRC)
	$(CC) -o minivim $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf obj/ minivim
