CC ?= cc
BIN ?= sphia
PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man/man1
MAN_FILES = $(wildcard man/*.md)
SOPHIA_SRCS = $(wildcard sophia/db/*.c)
REPL_SRCS = $(wildcard repl/*.c)
SRCS = $(SOPHIA_SRCS) $(REPL_SRCS) $(wildcard src/*.c)
TESTABLE_SRCS = $(filter-out src/main.c, $(SRCS))
TESTS = $(wildcard test/*.c)
CFLAGS += -std=c99 -Isophia/db -Irepl -Wall -Wextra -march=native -fPIC -fvisibility=hidden
LDFLAGS += -pthread

build: CFLAGS += -O2
build:
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS)

debug: CFLAGS += -g
debug: LDFLAGS += -DDEBUG
debug:
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS)

$(TESTS):
	$(CC) $(CFLAGS) $(TESTABLE_SRCS) $(@) -o $(basename $(@)) $(LDFLAGS)

test: $(TESTS)
	$(foreach e,$(TESTS:%.c=%),./$(basename $(e));)

install: $(MAN_FILES) build
	mkdir -p $(PREFIX)/bin
	install $(BIN) $(PREFIX)/bin
	mkdir -p $(MANPREFIX)
	install $(MAN_FILES:%.md=%.1) $(MANPREFIX)

uninstall:
	rm -f $(PREFIX)/bin/$(BIN) $(MANPREFIX)/$(MAN_FILES:man/%.md=%.1)

clean:
	rm -rf $(BIN) man/*.1 test-db test-sphia
	find test -type f -executable -exec rm -f {} \;

docs: $(MAN_FILES)
$(MAN_FILES):
	curl -# -F page=@$(@) -o $(@:%.md=%.1) http://mantastic.herokuapp.com

.PHONY: $(TESTS) test install uninstall clean docs $(MAN_FILES)
