CC ?= cc
BIN ?= sphia
PREFIX ?= /usr/local
MANPREFIX ?= $(PREFIX)/share/man/man1
MAN_FILES = $(wildcard man/*.md)
DEPS_OBJS = $(patsubst %.c,%.o,$(wildcard deps/**/*.c))
OBJS = $(filter-out src/main.o, $(patsubst %.c,%.o,$(wildcard src/*.c)))
TESTS = $(patsubst %.c,%,$(wildcard test/*.c))
CFLAGS += -std=c99 -Ideps -Wall -Wextra -march=native -fPIC -fvisibility=hidden
CPPFLAGS += -D_BSD_SOURCE -DBIN_NAME='"$(BIN)"'
LDFLAGS += -pthread -lsophia

build: CFLAGS += -O2
build: CPPFLAGS += -DNDEBUG
build: src/main

debug: CFLAGS += -g
debug: src/main

src/main: $(DEPS_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(?) $(@).c -o $(BIN) $(LDFLAGS)

$(TESTS): CFLAGS += -g
$(TESTS): $(DEPS_OBJS) $(OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(?) $(@).c -o $(@) $(LDFLAGS)

test: $(TESTS)
	@echo "\nRunning sphia tests"
	@$(foreach e, $(TESTS:%.c=%), ./$(basename $(e)) && echo "  ✓ $(e)";)
	@echo

travis:
	git clone --depth=1 https://github.com/larzconwell/sophia.git sophia
	$(MAKE) -C sophia/db
	mv sophia/db sophia/sophia
	rm -f sophia/sophia/*.so*
	CFLAGS="-Isophia/" LIBRARY_PATH="./sophia/sophia" $(MAKE) test

install: $(MAN_FILES) build
	mkdir -p $(PREFIX)/bin
	install $(BIN) $(PREFIX)/bin
	mkdir -p $(MANPREFIX)
	install $(MAN_FILES:%.md=%.1) $(MANPREFIX)

uninstall:
	rm -f $(PREFIX)/bin/$(BIN) $(MANPREFIX)/$(MAN_FILES:man/%.md=%.1)

clean:
	rm -rf $(DEPS_OBJS) $(OBJS) $(BIN) man/*.1 test-db test-sphia sophia
	find test -type f -perm +111 -exec rm -f {} \;
	rm -rf test/*.dSYM

docs: $(MAN_FILES)
$(MAN_FILES):
	curl -# -F page=@$(@) -o $(@:%.md=%.1) http://mantastic.herokuapp.com

.PHONY: test install uninstall clean docs $(MAN_FILES)
