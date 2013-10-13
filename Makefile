
SOPHIA_SRC = $(wildcard src/sophia/db/*.c)
SRC = $(SOPHIA_SRC) $(wildcard deps/*.c) $(wildcard src/*.c)
CFLAGS = -std=c99 -Isrc/sophia/db -Wall -march=native -fPIC -fvisibility=hidden  -O2

all: clean test build

clean:
	rm -f ./test-sphia

test:
	@:

build:
	$(CC) $(SRC) $(CFLAGS) -o sphia

.PHONY: all clean test build
