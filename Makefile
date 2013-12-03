CC ?= cc
BIN ?= sphia
PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man/man1
MAN_FILES = $(wildcard man/*.md)
REPL_OBJS = $(patsubst %.c,%.o,$(wildcard repl/*.c))
OBJS = $(filter-out src/main.o, $(patsubst %.c,%.o,$(wildcard src/*.c)))
TESTS = $(patsubst %.c,%,$(wildcard test/*.c))
CFLAGS += -std=c99 -Irepl -Wall -Wextra -march=native -fPIC -fvisibility=hidden
LDFLAGS += -pthread -lsophia

build: CFLAGS += -O2
build: src/main

debug: CFLAGS += -g
debug: LDFLAGS += -DDEBUG
debug: src/main

src/main: $(REPL_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(?) $(@).c -o $(BIN) $(LDFLAGS)

$(TESTS): $(REPL_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(?) $(@).c -o $(@) $(LDFLAGS)

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
	rm -rf $(REPL_OBJS) $(OBJS) $(BIN) man/*.1 test-db test-sphia
	find test -type f -executable -exec rm -f {} \;

docs: $(MAN_FILES)
$(MAN_FILES):
	curl -# -F page=@$(@) -o $(@:%.md=%.1) http://mantastic.herokuapp.com

.PHONY: test install uninstall clean docs $(MAN_FILES)
