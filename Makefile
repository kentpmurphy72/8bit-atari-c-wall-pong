ifneq ($(wildcard .tools/cc65/include/atari.h),)
export CC65_HOME := $(CURDIR)/.tools/cc65
endif

CL65 ?= $(if $(wildcard .tools/cc65/bin/cl65),.tools/cc65/bin/cl65,cl65)
SIM65 ?= $(if $(wildcard .tools/cc65/bin/sim65),.tools/cc65/bin/sim65,sim65)
CC ?= cc
CFLAGS = -O -t atari

.PHONY: all test test-6502 bootstrap clean
all: build/wall-pong.xex

build:
	mkdir -p build

build/wall-pong.xex: src/main.c src/game.c src/game.h | build
	$(CL65) $(CFLAGS) -m build/wall-pong.map -o $@ src/main.c src/game.c

test: | build
	$(CC) -std=c99 -Wall -Wextra -Werror -Isrc tests/game_test.c src/game.c -o build/game-test
	./build/game-test

clean:
	rm -rf build src/*.o

bootstrap:
	./tools/bootstrap-cc65.sh

test-6502: | build
	$(CL65) -t sim6502 -O -Isrc -o build/game-test-6502 tests/game_test.c src/game.c
	$(SIM65) build/game-test-6502
