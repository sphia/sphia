
BIN = sphia
PREFIX = /usr/local
MANPREFIX ?= "$(PREFIX)/share/man/man1"
SOPHIA_SRC = $(wildcard sophia/db/*.c)
REPL_SRC = $(wildcard repl/*.c)
SRC = $(SOPHIA_SRC) $(REPL_SRC) $(wildcard src/*.c)
TESTABLE_SOURCES = $(filter-out src/main.c, $(SRC))
TESTS = $(wildcard test/*.c)
CFLAGS = -std=c99 -Isophia/db -Irepl -Wall -pthread -march=native -fPIC -fvisibility=hidden  -O2

all: clean test build

clean:
	rm -fr ./sphia ./test-db ./test-sphia
	$(foreach test, $(TESTS), rm -f $(basename $(test));)

test: clean $(TESTS)

$(TESTS):
	@$(CC) $@ $(TESTABLE_SOURCES) $(CFLAGS) -o $(basename $@)
	./$(basename $@)

build:
	$(CC) $(SRC) $(CFLAGS) -o $(BIN)

install: build
	install $(BIN) $(PREFIX)/bin
	cp -f man/*.1 $(MANPREFIX)
	$(MAKE) clean

uninstall:
	rm -f $(PREFIX)/bin/$(BIN)

docs:
	rm -f man/sphia.1
	curl -F page=@man/sphia.md http://mantastic.herokuapp.com > man/sphia.1

.PHONY: all clean test build install uninstall $(TESTS)
