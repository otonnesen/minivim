CC = gcc
CFLAGS = -O2 -Wall -Wextra -pedantic -std=c99

TARGETS = buffer config editor keypress minivim terminal util

.PHONY: all check-style clean debug

OBJ = $(patsubst %,obj/%.o,$(TARGETS))
SRC = src/%.c

all: check-style minivim

obj/%.o: $(SRC) | obj
	$(CC) -c -o $@ $< $(CFLAGS)

obj:
	mkdir -p $@

minivim: $(OBJ)
	$(CC) -o minivim $^ $(CFLAGS)

debug: CFLAGS += -Og -g
debug: minivim

check-style:
	@for file in $$(ls src); do \
		var=`cat "src/$$file" | indent -linux -nlp -ci16 - | diff "src/$$file" - | wc -l` ; \
		if [ $$var -ne 0 ]; then \
			echo "$$file indent failed (diff: $$var lines)"; \
			exit 1; \
		fi; \
	done
	@echo "All files passed style check."

clean:
	rm -rf obj/ minivim
