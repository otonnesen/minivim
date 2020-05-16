CC = gcc
CFLAGS = -O -Wall -Wextra -pedantic -std=c99

TARGETS = buffer config editor minivim terminal util

SRC = $(patsubst %,obj/%.o,$(TARGETS))

all: compile

obj/%.o: src/%.c | obj
	$(CC) -c -o $@ $< $(CFLAGS)

obj:
	mkdir -p $@

.PHONY: compile

compile: $(SRC) check-style
	$(CC) -o minivim $(SRC) $(CFLAGS)

.PHONY: check-style

check-style:
	@for file in $$(ls src); do \
		var=`cat "src/$$file" | indent -linux -nlp -ci16 - | diff "src/$$file" - | wc -l` ; \
		if [ $$var -ne 0 ]; then \
			echo "$$file indent failed (diff: $$var lines)"; \
			exit 1; \
		fi; \
	done
	@echo "All files passed style check."

.PHONY: clean

clean:
	rm -rf obj/ minivim
