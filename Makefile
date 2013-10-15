
BIN = sphia
PREFIX = /usr/local
MANPREFIX ?= "$(PREFIX)/share/man/man1"
SOPHIA_SRC = $(wildcard sophia/db/*.c)
REPL_SRC = $(wildcard repl/*.c)
SRC = $(SOPHIA_SRC) $(REPL_SRC) $(wildcard src/*.c)
CFLAGS = -std=c99 -Isophia/db -Irepl -Wall -pthread -march=native -fPIC -fvisibility=hidden  -O2

all: clean test build

clean:
	rm -f ./sphia

test:
	@:

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

.PHONY: all clean test build install uninstall
