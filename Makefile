CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -O2
TOOLS   := minigrep/minigrep hexdump/hexdump csv2json/csv2json

all: $(TOOLS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	./tests.sh

clean:
	rm -f $(TOOLS) examples/*.json

.PHONY: all test clean
